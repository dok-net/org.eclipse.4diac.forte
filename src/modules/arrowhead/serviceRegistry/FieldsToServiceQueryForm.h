/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#ifndef _FIELDSTOSERVICEQUERYFORM_H_
#define _FIELDSTOSERVICEQUERYFORM_H_

#include <basicfb.h>
#include <forte_servicequeryform.h>
#include <forte_arrowheadservice.h>
#include <forte_bool.h>
#include <forte_dint.h>

class FORTE_FieldsToServiceQueryForm: public CBasicFB{
  DECLARE_FIRMWARE_FB(FORTE_FieldsToServiceQueryForm)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_ArrowheadService &service() {
    return *static_cast<CIEC_ArrowheadService*>(getDI(0));
  };

  CIEC_BOOL &metadataSearch() {
    return *static_cast<CIEC_BOOL*>(getDI(1));
  };

  CIEC_BOOL &pingProviders() {
    return *static_cast<CIEC_BOOL*>(getDI(2));
  };

  CIEC_DINT &version() {
    return *static_cast<CIEC_DINT*>(getDI(3));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_ServiceQueryForm &serviceQueryForm() {
    return *static_cast<CIEC_ServiceQueryForm*>(getDO(0));
  };

  static const TEventID scm_nEventREQID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventCNFID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_BASIC_FB_DATA_ARRAY(1, 4, 1, 0, 0);
  void alg_REQ();
  static const TForteInt16 scm_nStateSTART = 0;
  static const TForteInt16 scm_nStateREQ = 1;

  void enterStateSTART();
  void enterStateREQ();

  virtual void executeEvent(int pa_nEIID) override;

public:
  FORTE_FieldsToServiceQueryForm(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : 
       CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId,
              0, m_anFBConnData, m_anFBVarsData){
  };

  ~FORTE_FieldsToServiceQueryForm() override = default;

};

#endif //close the ifdef sequence from the beginning of the file

