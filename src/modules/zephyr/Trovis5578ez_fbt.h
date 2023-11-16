/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: Trovis5578ez
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2023-11-15/kaar -  -
 *************************************************************************/

#pragma once

#include "funcbloc.h"
#include "forte_bool.h"
#include "forte_wstring.h"
#include "forte_time.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"


class FORTE_Trovis5578ez final : public CFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_Trovis5578ez)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
    static const TEventID scmEventINITID = 0;
    static const TDataIOID scmEIWith[];
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventINDID = 1;
    static const TDataIOID scmEOWith[];
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

    void deregister_handles();
    void register_handles();

  public:
    FORTE_Trovis5578ez(CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes);

    CIEC_BOOL var_QI;
    CIEC_WSTRING var_IN1;
    CIEC_WSTRING var_OUT1;
    CIEC_WSTRING var_OUT2;
    CIEC_TIME var_UpdateInterval;

    CIEC_BOOL var_QO;
    CIEC_WSTRING var_STATUS;

    CIEC_BOOL var_conn_QO;
    CIEC_WSTRING var_conn_STATUS;

    CEventConnection conn_INITO;
    CEventConnection conn_IND;

    CDataConnection *conn_QI;
    CDataConnection *conn_IN1;
    CDataConnection *conn_OUT1;
    CDataConnection *conn_OUT2;
    CDataConnection *conn_UpdateInterval;

    CDataConnection conn_QO;
    CDataConnection conn_STATUS;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_INIT(const CIEC_BOOL &paQI, const CIEC_WSTRING &paIN1, const CIEC_WSTRING &paOUT1, const CIEC_WSTRING &paOUT2, const CIEC_TIME &paUpdateInterval, CIEC_BOOL &paQO, CIEC_WSTRING &paSTATUS) {
      var_QI = paQI;
      var_IN1 = paIN1;
      var_OUT1 = paOUT1;
      var_OUT2 = paOUT2;
      var_UpdateInterval = paUpdateInterval;
      executeEvent(scmEventINITID, nullptr);
      paQO = var_QO;
      paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paQI, const CIEC_WSTRING &paIN1, const CIEC_WSTRING &paOUT1, const CIEC_WSTRING &paOUT2, const CIEC_TIME &paUpdateInterval, CIEC_BOOL &paQO, CIEC_WSTRING &paSTATUS) {
      evt_INIT(paQI, paIN1, paOUT1, paOUT2, paUpdateInterval, paQO, paSTATUS);
    }
};


