#include "CallbacksRequestPurchase.h"
#include "CallbackSingleton.h"

#include "ODK_internal.h"
#include "s3eEdk.h"
#include "s3eEdk_android.h"
#include "IwDebug.h"

#include <stdio.h>

void CallbacksRequestPurchase::RegisterCallback(s3eCallback callback, s3eCallback* savedCallback, int callbackType)
{
	if (*savedCallback)
	{
		//IwTrace(ODK, ("Unregistering Callback"));

		s3eEdkCallbacksUnRegister(
				S3E_EXT_ODK_HASH,
				S3E_ODK_CALLBACKS_MAX,
				callbackType,
				*savedCallback,
				NULL);
		*savedCallback = NULL;
	}

	*savedCallback = callback;

	//IwTrace(ODK, ("Registering Callback"));

	s3eEdkCallbacksRegister(
			S3E_EXT_ODK_HASH,
			S3E_ODK_CALLBACKS_MAX,
			callbackType,
			*savedCallback,
			NULL,
			S3E_FALSE);
}

void CallbacksRequestPurchase::RegisterCallbacks(s3eCallback onSuccess, s3eCallback onFailure, s3eCallback onCancel)
{
	RegisterCallback(onSuccess, &m_onSuccess, S3E_ODK_CALLBACKS_REQUEST_PURCHASE_ON_SUCCESS);
	RegisterCallback(onFailure, &m_onFailure, S3E_ODK_CALLBACKS_REQUEST_PURCHASE_ON_FAILURE);
	RegisterCallback(onCancel, &m_onCancel, S3E_ODK_CALLBACKS_REQUEST_PURCHASE_ON_CANCEL);
}

void CallbacksRequestPurchase::OnSuccess(const OuyaSDK::Product& product)
{
	IwTrace(ODK, ("OnSuccess"));

	s3eRequestPurchaseSuccessEvent event;
	event.m_product = product;

	m_dataRequestPurchaseSuccessEvent = event; //don't send a temp pointer
	s3eEdkCallbacksEnqueue(S3E_EXT_ODK_HASH, S3E_ODK_CALLBACKS_REQUEST_PURCHASE_ON_SUCCESS, &m_dataRequestPurchaseSuccessEvent);
}

void CallbacksRequestPurchase::OnFailure(int errorCode, const std::string& errorMessage)
{
	char buffer[256];
	sprintf(buffer, "OnFailure errorCode=%d errorMessage=%s", errorCode, errorMessage.c_str());
	IwTrace(ODK, (buffer));

	s3eRequestPurchaseFailureEvent event;
	event.m_errorCode = errorCode;
	sprintf(event.m_errorMessage, "%s", errorMessage.c_str());

	m_dataRequestPurchaseFailureEvent = event; //don't send a temp pointer
	s3eEdkCallbacksEnqueue(S3E_EXT_ODK_HASH, S3E_ODK_CALLBACKS_REQUEST_PURCHASE_ON_FAILURE, &m_dataRequestPurchaseFailureEvent);
}

void CallbacksRequestPurchase::OnCancel()
{
	IwTrace(ODK, ("OnCancel"));

	s3eRequestPurchaseCancelEvent event;

	m_dataRequestPurchaseCancelEvent = event; //don't send a temp pointer
	s3eEdkCallbacksEnqueue(S3E_EXT_ODK_HASH, S3E_ODK_CALLBACKS_REQUEST_PURCHASE_ON_CANCEL, &m_dataRequestPurchaseCancelEvent);
}