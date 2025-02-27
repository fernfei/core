#include "CElement.h"
#include <algorithm>
#include <math.h>

#include "StaticFunctions.h"

namespace NSCSS
{
	CElement::CElement()
	{
	}
	CElement::~CElement()
	{
		for (CElement* oElement : m_arPrevElements)
			if (oElement != NULL)
				delete oElement;
			else
				continue;

		m_mStyle.clear();

	}

	std::wstring CElement::GetSelector() const
	{
		return m_sSelector;
	}

	std::wstring CElement::GetFullSelector() const
	{
		return m_sFullSelector;
	}

	bool CElement::Empty() const
	{
		return m_mStyle.empty();
	}

	void CElement::SetSelector(const std::wstring &sSelector)
	{
		m_sSelector = sSelector;
		m_sFullSelector = m_sSelector;
		UpdateWeight();
	}

	void NSCSS::CElement::AddPropertie(const std::wstring &sName, const std::wstring& sValue)
	{
		m_mStyle[sName] = sValue;
	}

	void CElement::AddProperties(const std::map<std::wstring, std::wstring>& mProperties)
	{
		for (std::pair<std::wstring, std::wstring> pPropertie : mProperties)
		{
			std::transform(pPropertie.first.begin(), pPropertie.first.end(), pPropertie.first.begin(), tolower);

			if (!m_mStyle[pPropertie.first].empty() && m_mStyle[pPropertie.first].find(L'!') != std::wstring::npos)
				continue;

			m_mStyle[pPropertie.first] = pPropertie.second;
		}
	}

	void CElement::AddPrevElement(CElement *oPrevElement)
	{
		if (NULL == oPrevElement)
			return;

		m_arPrevElements.push_back(oPrevElement);
		oPrevElement->m_sFullSelector += L' ' + m_sFullSelector;
		UpdateWeight();
	}

	void CElement::AddKinElement(CElement *oKinElement)
	{
		if (NULL == oKinElement)
			return;

		m_arKinElements.push_back(oKinElement);
		oKinElement->m_sFullSelector += m_sFullSelector;
		oKinElement->UpdateWeight();
	}

	std::map<std::wstring, std::wstring> CElement::GetStyle() const
	{
		return m_mStyle;
	}

	std::map<std::wstring, std::wstring> CElement::GetFullStyle(const std::vector<CNode>& arSelectors) const
	{
		if (arSelectors.empty())
			return std::map<std::wstring, std::wstring>();

		std::map<std::wstring, std::wstring> mStyle(m_mStyle);

//        Print();
//        std::wcout << L"++++++++++++++++++++++++++++++++++++++++" << std::endl;

		for (const CElement* oElement : m_arPrevElements)
		{
			for (std::vector<CNode>::const_reverse_iterator oNode = arSelectors.rbegin(); oNode != arSelectors.rend(); ++oNode)
			{
				std::map<std::wstring, std::wstring> mTempStyle;
                                if (oElement->GetSelector() == L'.' + oNode->m_wsClass ||
					oElement->GetSelector() == oNode->m_wsName)
				{
					for (const std::pair<std::wstring, std::wstring> pPropertie : oElement->GetFullStyle(arSelectors))
						mStyle[pPropertie.first] = pPropertie.second;
//                    const std::map<std::wstring, std::wstring> mTempStyle = oElement->GetFullStyle(arSelectors);
//                    mStyle.insert(mTempStyle.begin(), mTempStyle.end());
				}
			}
		}

		return mStyle;
	}

	std::map<std::wstring, std::wstring> CElement::GetFullStyle(const std::vector<std::wstring> &arNodes) const
	{
		if (arNodes.empty() || (m_mStyle.empty() && m_arPrevElements.empty()))
			return std::map<std::wstring, std::wstring>();

		std::map<std::wstring, std::wstring> mStyle(m_mStyle);

		for (const CElement* oElement : m_arPrevElements)
		{
			for (std::vector<std::wstring>::const_reverse_iterator sNode = arNodes.rbegin(); sNode != arNodes.rend(); ++sNode)
			{
				if ((*sNode)[0] == L'.')
				{
					for (const std::wstring& sClass : NS_STATIC_FUNCTIONS::GetWordsW(*sNode, false,  L" "))
					{
						if (oElement->GetSelector() == sClass)
						{
							std::vector<std::wstring> sTempNodes = arNodes;
							sTempNodes.pop_back();

							for (const std::pair<std::wstring, std::wstring> pPropertie : oElement->GetFullStyle(sTempNodes))
								mStyle[pPropertie.first] = pPropertie.second;
						}
					}
				}
				else if (oElement->GetSelector() == *sNode)
				{
					std::vector<std::wstring> sTempNodes = arNodes;
					sTempNodes.pop_back();
					for (const std::pair<std::wstring, std::wstring> pPropertie : oElement->GetFullStyle(sTempNodes))
						mStyle[pPropertie.first] = pPropertie.second;
				}
			}
		}

		return mStyle;
	}

	std::vector<CElement *> CElement::GetNextOfKin(const std::wstring &sName, const std::vector<std::wstring>& arClasses) const
	{
		if (m_arKinElements.empty())
			return std::vector<CElement*>();

		std::vector<CElement*> arElements;
		for (CElement* oElement : m_arKinElements)
		{
			if (!sName.empty() && oElement->m_sSelector == sName)
			{
				arElements.push_back(oElement);
				continue;
			}

			for (const std::wstring& sClass : arClasses)
			{
				if (oElement->m_sSelector == sClass)
					arElements.push_back(oElement);
			}
		}
		return arElements;
	}

	std::vector<CElement *> CElement::GetPrevElements(const std::vector<std::wstring>::const_reverse_iterator& oNodesRBegin, const std::vector<std::wstring>::const_reverse_iterator& oNodesREnd) const
	{
		if (oNodesRBegin >= oNodesREnd || m_arPrevElements.empty())
			return std::vector<CElement*>();

		std::vector<CElement*> arElements;

		for (std::vector<std::wstring>::const_reverse_iterator iWord = oNodesRBegin; iWord != oNodesREnd; ++iWord)
		{
			if ((*iWord)[0] == L'.' && ((*iWord).find(L" ") != std::wstring::npos))
			{
				std::vector<std::wstring> arClasses = NS_STATIC_FUNCTIONS::GetWordsW(*iWord, false, L" ");
				for (const std::wstring& wsClass : arClasses)
				{
					for (CElement* oPrevElement : m_arPrevElements)
					{
						if (oPrevElement->m_sSelector == wsClass)
						{
							arElements.push_back(oPrevElement);
							std::vector<CElement*> arTempElements = oPrevElement->GetPrevElements(iWord + 1, oNodesREnd);
							arElements.insert(arElements.end(), arTempElements.begin(), arTempElements.end());
						}
					}
				}
			}
			else
			{
				for (CElement* oPrevElement : m_arPrevElements)
				{
					if (oPrevElement->m_sSelector == *iWord)
					{
						arElements.push_back(oPrevElement);
						std::vector<CElement*> arTempElements = oPrevElement->GetPrevElements(iWord + 1, oNodesREnd);
						arElements.insert(arElements.end(), arTempElements.begin(), arTempElements.end());
	//                    return arElements;
					}
				}
			}
		}

		return arElements;

	}

	CElement *CElement::FindPrevElement(const std::wstring &sSelector) const
	{
		if (sSelector.empty())
			return NULL;

		for (CElement* oElement : m_arPrevElements)
		{
			if (oElement->m_sSelector == sSelector)
				return oElement;
		}
		return NULL;
	}

	void CElement::UpdateWeight()
	{
		if (m_arWeight.empty())
			m_arWeight = NS_STATIC_FUNCTIONS::GetWeightSelector(m_sFullSelector);
	}

	std::vector<unsigned short> CElement::GetWeight() const
	{
		return m_arWeight;
	}

	void CElement::IncreasedWeight()
	{
		if (!m_arWeight.empty())
		{
			m_arWeight = NS_STATIC_FUNCTIONS::GetWeightSelector(m_sFullSelector);
			++m_arWeight[3];
		}
	}
}

