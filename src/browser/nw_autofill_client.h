// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NW_BROWSER_UI_AUTOFILL_CHROME_AUTOFILL_CLIENT_H_
#define NW_BROWSER_UI_AUTOFILL_CHROME_AUTOFILL_CLIENT_H_

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/i18n/rtl.h"
#include "base/memory/weak_ptr.h"
#include "components/autofill/core/browser/autofill_client.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

namespace content {
struct FrameNavigateParams;
struct LoadCommittedDetails;
class WebContents;
}

namespace autofill {

class AutofillDialogController;
class AutofillKeystoneBridgeWrapper;
class AutofillPopupControllerImpl;
struct FormData;

// Chrome implementation of AutofillClient.
class NWAutofillClient
    : public AutofillClient,
      public content::WebContentsUserData<NWAutofillClient>,
      public content::WebContentsObserver {
 public:
  virtual ~NWAutofillClient();

  // Called when the tab corresponding to |this| instance is activated.
  void TabActivated();

  // AutofillClient:
  virtual PersonalDataManager* GetPersonalDataManager() override;
  virtual scoped_refptr<AutofillWebDataService> GetDatabase() override;
  virtual PrefService* GetPrefs() override;
  virtual void HideRequestAutocompleteDialog() override;
  virtual void ShowAutofillSettings() override;
  virtual void ConfirmSaveCreditCard(
      const AutofillMetrics& metric_logger,
      const base::Closure& save_card_callback) override;
  virtual void ShowRequestAutocompleteDialog(
      const FormData& form,
      const GURL& source_url,
      const ResultCallback& callback) override;
  virtual void ShowAutofillPopup(
      const gfx::RectF& element_bounds,
      base::i18n::TextDirection text_direction,
      const std::vector<base::string16>& values,
      const std::vector<base::string16>& labels,
      const std::vector<base::string16>& icons,
      const std::vector<int>& identifiers,
      base::WeakPtr<AutofillPopupDelegate> delegate) override;
  virtual void UpdateAutofillPopupDataListValues(
      const std::vector<base::string16>& values,
      const std::vector<base::string16>& labels) override;
  virtual void HideAutofillPopup() override;
  virtual bool IsAutocompleteEnabled() override;
  virtual void DetectAccountCreationForms(
      const std::vector<autofill::FormStructure*>& forms) override;
  virtual void DidFillOrPreviewField(
      const base::string16& autofilled_value,
      const base::string16& profile_full_name) override;

  // content::WebContentsObserver implementation.
  virtual void WebContentsDestroyed() override;

 private:
#if defined(OS_MACOSX) && !defined(OS_IOS)
  // Creates |bridge_wrapper_|, which is responsible for dealing with Keystone
  // notifications.
  void RegisterForKeystoneNotifications();

  // Deletes |bridge_wrapper_|.
  void UnregisterFromKeystoneNotifications();
#endif  // defined(OS_MACOSX) && !defined(OS_IOS)

  explicit NWAutofillClient(content::WebContents* web_contents);
  friend class content::WebContentsUserData<NWAutofillClient>;

  content::WebContents* const web_contents_;
  // base::WeakPtr<AutofillDialogController> dialog_controller_;
  base::WeakPtr<AutofillPopupControllerImpl> popup_controller_;

#if defined(OS_MACOSX) && !defined(OS_IOS)
  // Listens to Keystone notifications and passes relevant ones on to the
  // PersonalDataManager.
  //
  // The class of this member must remain a forward declaration, even in the
  // .cc implementation file, since the class is defined in a Mac-only
  // implementation file. This means that the pointer cannot be wrapped in a
  // scoped_ptr.
  // AutofillKeystoneBridgeWrapper* bridge_wrapper_;
#endif  // defined(OS_MACOSX) && !defined(OS_IOS)

  DISALLOW_COPY_AND_ASSIGN(NWAutofillClient);
};

}  // namespace autofill

#endif  // CHROME_BROWSER_UI_AUTOFILL_CHROME_AUTOFILL_CLIENT_H_