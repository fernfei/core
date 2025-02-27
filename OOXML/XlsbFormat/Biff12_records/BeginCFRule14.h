﻿/*
 * (c) Copyright Ascensio System SIA 2010-2021
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/BiffRecord.h"
#include "../../XlsxFormat/WritingElement.h"

#include "../Biff12_structures/CFType.h"
#include "../Biff12_structures/CFTemp.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/DXFId.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/CFParsedFormula.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BIFF12/FRTHeader.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BIFF12/XLWideString.h"

namespace XLSB
{
    // Logical representation of BrtBeginCFRule14 record in BIFF12
    class BeginCFRule14: public XLS::BiffRecord
    {
            BIFF_RECORD_DEFINE_TYPE_INFO(BeginCFRule14)
            BASE_OBJECT_DEFINE_CLASS_NAME(BeginCFRule14)
        public:
            BeginCFRule14();
            ~BeginCFRule14();

            XLS::BaseObjectPtr clone();

            void readFields(XLS::CFRecord& record) override;
			void writeFields(XLS::CFRecord& record) override;

             static const XLS::ElementType	type = XLS::typeBeginCFRule14;

			FRTHeader               FRTheader;
            CFType                  iType;
            CFTemp                  iTemplate;
            _UINT32                 dxfId = 0;
            _INT32                  iPri = 0xFFFFFFFF;
            _UINT32                 iParam = 0;
            std::string             sTypeiParam;
            bool                    fStopTrue = false;
            bool                    fAbove = false;
            bool                    fBottom = false;
            bool                    fPercent = false;
            _UINT32                 cbFmla1 = 0;
            _UINT32                 cbFmla2 = 0;
            _UINT32                 cbFmla3 = 0;
            std::wstring            guid;
            XLS::Boolean<unsigned int>  fGuid = false;
            XLNullableWideString    strParam;

    };

} // namespace XLSB

