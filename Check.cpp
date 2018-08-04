//============================================================================
// Name        : Check.cpp
// Author      : kdjie
// Version     : 1.0
// Copyright   : @2015
// Description : 14166097@qq.com
//============================================================================

#include "Check.h"

CCheck::CCheck(const CHands& hands)
{
	const CARDS_t& handCards = hands.getHandCards();
	const EATCARDS_t& eatCards = hands.getEatCards();

	setHandCards(handCards);
	setEatCards(eatCards);
}

// 设置鬼牌
void CCheck::setGostCard(const Card& card)
{
	m_local.m_cardsGost.push_back(card);
}
void CCheck::setGostCard(const CARDS_t& cards)
{
	m_local.m_cardsGost = cards;
}

// 设置是否支持相关胡牌类型
void CCheck::setSupport7Duizi(bool b)
{
	m_local.m_bSupport7Duizi = b;
}
void CCheck::setSupportPengpeng(bool b)
{
	m_local.m_bSupportPengpeng = b;
}
void CCheck::setSupportQingyise(bool b)
{
	m_local.m_bSupportQingyise = b;
}
void CCheck::setSupportQuanqiuren(bool b)
{
	m_local.m_bSupportQuanqiuren = b;
}

// 设置手上的牌
void CCheck::setHandCards(const CARDS_t& cards)
{
	m_handCards = cards;

	__calcCardCount();
}

// 变更手上的牌
void CCheck::addHandCard(const Card& card)
{
	if (!card.isValid())
		return;

	m_handCards.push_back(card);

	__opCardCount(card, 1);
}
void CCheck::delHandCard(const Card& card)
{
	if (!card.isValid())
		return;

	CARDS_t::iterator iter = std::find(m_handCards.begin(), m_handCards.end(), card);
	if (iter != m_handCards.end())
	{
		m_handCards.erase(iter);

		__opCardCount(card, -1);
	}
}

// 设置吃出去的牌
void CCheck::setEatCards(const EATCARDS_t& eats)
{
	m_eatCards = eats;
}

// 检测能否吃牌，返回可选牌型
bool CCheck::canEat(const Card& card, EATCARDS_t& eats)
{
	if (!card.isValid())
		return false;

	bool bRet = false;

	if (card.getPoint() <= 7)
	{
		if ( __getCardCount( card + 1 ) > 0 && __getCardCount( card + 2 ) > 0 )
		{
			SEatCard eat;
			eat.type = ectEat;
			eat.firstCard = card;
			eat.eatCard = card;
			eats.push_back(eat);

			bRet = true;
		}
	}

	if (card.getPoint() >= 3)
	{
		if ( __getCardCount( card - 2 ) > 0 && __getCardCount( card - 1 ) > 0 )
		{
			SEatCard eat;
			eat.type = ectEat;
			eat.firstCard = card - 2;
			eat.eatCard = card;
			eats.push_back(eat);

			bRet = true;
		}
	}

	if (card.getPoint() > 1 && card.getPoint() < 9 )
	{
		if ( __getCardCount( card - 1 ) > 0 && __getCardCount( card + 1 ) > 0 )
		{
			SEatCard eat;
			eat.type = ectEat;
			eat.firstCard = card - 1;
			eat.eatCard = card;
			eats.push_back(eat);

			bRet = true;
		}
	}

	return bRet;
}

// 检测能否碰牌
bool CCheck::canPeng(const Card& card)
{
	if (!card.isValid())
		return false;

	return (__getCardCount( card ) >= 2);
}

// 检测能否补牌
bool CCheck::canBu(const Card& card)
{
	if (!card.isValid())
		return false;

	return ( __getCardCount(card) == 3 );
}

bool CCheck::canBu(EATCARDS_t& eats, const CARDS_t& cardsPassPeng)
{
	// 判断手上的牌
	for (MAP_CARD_COUNT_t::iterator iter = m_mapCardCount.begin(); iter != m_mapCardCount.end(); ++iter)
	{
		if (iter->second == 4)
		{
			SEatCard eat;
			eat.type = ectAnGang;
			eat.firstCard = iter->first;
			eat.eatCard = iter->first;
			eats.push_back(eat);
		}
	}

	// 注释以下代码，原因：
	// 就是有些玩家，他手里有3个六条，然后别人打了一个六条。本来他可以补的。补的话，打出6条的那个人出3分的。然后这个人他就碰，然后补，搞得每人出3分。所以就吵起来
	// 解决问题就是：当轮他手里有3个六条，要么碰，要么补。不允许他碰了6条立马又补。

	// 判断吃出去的牌
	for (EATCARDS_t::iterator iter = m_eatCards.begin(); iter != m_eatCards.end(); ++iter)
	{
		if (iter->type == ectPeng && __getCardCount(iter->firstCard) == 1)
		{
			bool bFind = false;

			for (CARDS_t::const_iterator c_iterPass = cardsPassPeng.begin(); c_iterPass != cardsPassPeng.end(); ++c_iterPass)
			{
				if (c_iterPass->isValid() && *c_iterPass == iter->firstCard)
				{
					bFind = true;
					break;
				}
			}

			// 跳补
			if (bFind)
				continue;

			SEatCard eat;
			eat.type = ectGang;
			eat.firstCard = iter->firstCard;
			eat.eatCard = iter->firstCard;
			eats.push_back(eat);
		}
	}

	return (!eats.empty());
}

// 检测能否胡牌
bool CCheck::canHu(const Card& card, EATCARDS_t& eats)
{
	if (!card.isValid())
		return false;
 
	addHandCard(card);

	EATCARDS_t _eats;
	bool bRet = canHu(_eats);

	if (bRet)
	{
		for (EATCARDS_t::iterator iter = _eats.begin(); iter != _eats.end(); ++iter)
		{
			if (iter->type == ectEat && (iter->firstCard == card || iter->firstCard+1 == card || iter->firstCard+2 == card) )
			{
				SEatCard eat;
				eat.type = ectEat;
				eat.firstCard = iter->firstCard;
				eat.eatCard = card;
				eats.push_back(eat);
			}

			if (iter->type == ectPeng && iter->firstCard == card)
			{
				SEatCard eat;
				eat.type = ectPeng;
				eat.firstCard = iter->firstCard;
				eat.eatCard = card;
				eats.push_back(eat);
			}

			if (iter->type == ectDui && iter->firstCard == card)
			{
				SEatCard eat;
				eat.type = ectDui;
				eat.firstCard = iter->firstCard;
				eat.eatCard = card;
				eats.push_back(eat);
			}
		}
	}

	delHandCard(card);

	return bRet;
}

bool CCheck::canHu(EATCARDS_t& eats)
{
	m_local.reset();

	// 检查特殊胡牌类型

	// 支持7对子
	if (m_local.m_bSupport7Duizi)
	{
		int n4Count = 0;
		if (__is7Duizi(eats, n4Count))
		{
			//if (n4Count == 0)
			//	m_local.m_setHuType.insert(E_HU_7DUIZI);
			//else if (n4Count == 1)
			//	m_local.m_setHuType.insert(E_HU_H7DUIZI);
			//else if (n4Count == 2)
			//	m_local.m_setHuType.insert(E_HU_2H7DUIZI);
			//else if (n4Count == 3)
			//	m_local.m_setHuType.insert(E_HU_3H7DUIZI);
			m_local.m_setHuType.insert(E_HU_7DUIZI);
		}
	}

	// 支持碰碰胡
	if (m_local.m_bSupportPengpeng)
	{
		if (__isPengpengHu(eats))
		{
			m_local.m_setHuType.insert(E_HU_PENGPENG);
		}
	}

	// 支持全求人
	if (m_local.m_bSupportQuanqiuren)
	{
		if (__isQuanqiuren(eats))
		{
			m_local.m_setHuType.insert(E_HU_QUANQIUREN);
		}
	}

	// 若无胡牌，检查普通胡牌类型

	if (m_local.m_setHuType.empty())
	{
		if (__checkHu(eats))
		{
			m_local.m_setHuType.insert(E_HU_XIAO);
		}
	}

	// 胡牌后，检查附加属性

	if (!m_local.m_setHuType.empty())
	{
		// 判断清一色
		if (m_local.m_bSupportQingyise)
		{
			if (__isQingyise())
			{
				m_local.m_setHuType.insert(E_HU_QINGYISE);
				// 当有清一色时，消除小胡
				m_local.m_setHuType.erase(E_HU_XIAO);
			}
		}

		return true;
	}

	return false;
}

// 获取胡牌类型
std::set<int> CCheck::getHuTypes()
{
	return m_local.m_setHuType;
}

// 检测能否听牌
bool CCheck::canTing()
{
	CARDS_t cards;
	return canTing(cards);
}

bool CCheck::canTing(CARDS_t& cards)
{
	// 普通子
	for (int color = MJ_COLOR_WZ; color <= MJ_COLOR_TZ; ++color)
	{
		for (int point = 1; point <= 9; ++point)
		{
			Card card(color, point);

			if (__getCardCount(card) == 4)
				continue;

			EATCARDS_t eats;
			if ( canHu(card, eats) )
			{
				cards.push_back(card);
			}
		}
	}

	// 风子
	for (int color = MJ_COLOR_FZ; color <= MJ_COLOR_FZ; ++color)
	{
		for (int point = FZ_POINT_HONGZHONG; point <= FZ_POINT_BAIBAN; ++point)
		{
			Card card(color, point);

			if (__getCardCount(card) == 4)
				continue;

			EATCARDS_t eats;
			if ( canHu(card, eats) )
			{
				cards.push_back(card);
			}
		}
	}

	return (!cards.empty());
}

// 统计每种牌的张数
void CCheck::__calcCardCount()
{
	m_mapCardCount.clear();

	for (CARDS_t::iterator iter = m_handCards.begin(); iter != m_handCards.end(); ++iter)
	{
		m_mapCardCount[ *iter ]++;
	}
}

// 获取每种牌的张数
int CCheck::__getCardCount(const Card& card)
{
	MAP_CARD_COUNT_t::iterator iter = m_mapCardCount.find(card);
	if (iter == m_mapCardCount.end())
		return 0;

	return iter->second;
}

// 操作每种牌增减
void CCheck::__opCardCount(const Card& card, int cnt)
{
	m_mapCardCount[card] += cnt;

	if (m_mapCardCount[card] == 0)
	{
		m_mapCardCount.erase(card);
	}
}

Card CCheck::__remainCard()
{
	CARDS_t delayCards;

	for (MAP_CARD_COUNT_t::iterator iter = m_mapCardCount.begin(); iter != m_mapCardCount.end(); ++iter)
	{
		// 鬼牌往后靠
		if (__isGost(iter->first))
		{
			delayCards.push_back(iter->second);
			continue;
		}

		return iter->first;
	}

	for (CARDS_t::iterator iter = delayCards.begin(); iter != delayCards.end(); ++iter)
	{
		return (*iter);
	}

	return Card();
}

bool CCheck::__checkHu(EATCARDS_t& eats)
{
	Card card = __remainCard();

	if (!card.isValid())
		return true;

	int theCardCount = __getCardCount(card);

	// 处理刻子
	{
		// 不填充
		if (theCardCount >= 3)
		{
			__opCardCount(card, -3);

			bool bRet = __checkHu(eats);

			__opCardCount(card, 3);

			if (bRet)
			{
				SEatCard eat;
				eat.type = ectPeng;
				eat.firstCard = card;
				eat.eatCard = card;

				// 真实牌型
				eat.realCards.push_back(card);
				eat.realCards.push_back(card);
				eat.realCards.push_back(card);

				eats.push_back(eat);

				return true;
			}
		}

		// 填充1子
		if (theCardCount >= 2)
		{
			std::map<Card, int> mapGostCount;
			__getGostAndCount(mapGostCount);

			// 找1个鬼
			for (std::map<Card, int>::iterator iter = mapGostCount.begin(); iter != mapGostCount.end(); ++iter)
			{
				Card theCardFillGost = iter->first;

				if (theCardFillGost == card || iter->second < 1)
					continue;

				__opCardCount(card, -1);
				__opCardCount(card, -1);
				__opCardCount(theCardFillGost, -1);

				bool bRet = __checkHu(eats);

				__opCardCount(card, 1);
				__opCardCount(card, 1);
				__opCardCount(theCardFillGost, 1);

				if (bRet)
				{
					SEatCard eat;
					eat.type = ectPeng;
					eat.firstCard = card;
					eat.eatCard = card;

					// 真实牌型
					eat.realCards.push_back(card);
					eat.realCards.push_back(card);
					eat.realCards.push_back(theCardFillGost);

					eats.push_back(eat);

					return true;
				}
			}
		}

		// 填充2子
		if (theCardCount >= 1)
		{
			std::map<Card, int> mapGostCount;
			__getGostAndCount(mapGostCount);

			// 找2个同鬼
			for (std::map<Card, int>::iterator iter = mapGostCount.begin(); iter != mapGostCount.end(); ++iter)
			{
				Card theCardFillGost = iter->first;

				if (theCardFillGost == card || iter->second < 2)
					continue;

				__opCardCount(card, -1);
				__opCardCount(theCardFillGost, -1);
				__opCardCount(theCardFillGost, -1);

				bool bRet = __checkHu(eats);

				__opCardCount(card, 1);
				__opCardCount(theCardFillGost, 1);
				__opCardCount(theCardFillGost, 1);

				if (bRet)
				{
					SEatCard eat;
					eat.type = ectPeng;
					eat.firstCard = card;
					eat.eatCard = card;

					// 真实牌型
					eat.realCards.push_back(card);
					eat.realCards.push_back(theCardFillGost);
					eat.realCards.push_back(theCardFillGost);

					eats.push_back(eat);

					return true;
				}
			}

			// 找2个异鬼
			for (std::map<Card, int>::iterator iter1 = mapGostCount.begin(); iter1 != mapGostCount.end(); ++iter1)
			{
				Card theCardFillGost1 = iter1->first;

				if (theCardFillGost1 == card || iter1->second < 1)
					continue;

				for (std::map<Card, int>::iterator iter2 = mapGostCount.begin(); iter2 != mapGostCount.end(); ++iter2)
				{
					Card theCardFillGost2 = iter2->first;

					if (theCardFillGost2 == card || theCardFillGost2 == theCardFillGost1 || iter2->second < 1)
						continue;

					__opCardCount(card, -1);
					__opCardCount(theCardFillGost1, -1);
					__opCardCount(theCardFillGost2, -1);

					bool bRet = __checkHu(eats);

					__opCardCount(card, 1);
					__opCardCount(theCardFillGost1, 1);
					__opCardCount(theCardFillGost2, 1);

					if (bRet)
					{
						SEatCard eat;
						eat.type = ectPeng;
						eat.firstCard = card;
						eat.eatCard = card;

						// 真实牌型
						eat.realCards.push_back(card);
						eat.realCards.push_back(theCardFillGost1);
						eat.realCards.push_back(theCardFillGost2);

						eats.push_back(eat);

						return true;
					}
				}
			}
		}
	}

	// 处理将/乱将
	if (!m_local.m_bDui)
	{
		// 不填充
		if (theCardCount >= 2)
		{
			m_local.m_bDui = true;

			__opCardCount(card, -1);
			__opCardCount(card, -1);

			bool bRet = __checkHu(eats);

			__opCardCount(card, 1);
			__opCardCount(card, 1);

			if (bRet)
			{
				SEatCard eat;
				eat.type = ectDui;
				eat.firstCard = card;
				eat.eatCard = card;

				// 真实牌型
				eat.realCards.push_back(card);
				eat.realCards.push_back(card);

				eats.push_back(eat);

				return true;
			}
			else
			{
				m_local.m_bDui = false;
			}
		}

		// 填充1子
		if (theCardCount >= 1)
		{
			std::map<Card, int> mapGostCount;
			__getGostAndCount(mapGostCount);

			// 找1个鬼
			for (std::map<Card, int>::iterator iter = mapGostCount.begin(); iter != mapGostCount.end(); ++iter)
			{
				Card theCardFillGost = iter->first;

				if (theCardFillGost == card || iter->second < 1)
					continue;

				m_local.m_bDui = true;

				__opCardCount(card, -1);
				__opCardCount(theCardFillGost, -1);

				bool bRet = __checkHu(eats);

				__opCardCount(card, 1);
				__opCardCount(theCardFillGost, 1);

				if (bRet)
				{
					SEatCard eat;
					eat.type = ectDui;
					eat.firstCard = card;
					eat.eatCard = card;

					// 真实牌型
					eat.realCards.push_back(card);
					eat.realCards.push_back(theCardFillGost);

					eats.push_back(eat);

					return true;
				}
				else
				{
					m_local.m_bDui = false;
				}
			}
		}
	}

	// 处理顺子
	if (card.getColor() != MJ_COLOR_FZ)
	{
		// 不填充
		if (card.getPoint() <= 7 && __getCardCount(card+1) > 0 && __getCardCount(card+2) > 0)
		{
			__opCardCount(card, -1);
			__opCardCount(card+1, -1);
			__opCardCount(card+2, -1);

			bool bRet = __checkHu(eats);

			__opCardCount(card, 1);
			__opCardCount(card+1, 1);
			__opCardCount(card+2, 1);

			if (bRet)
			{
				SEatCard eat;
				eat.type = ectEat;
				eat.firstCard = card;
				eat.eatCard = card;

				// 真实牌型
				eat.realCards.push_back(card);
				eat.realCards.push_back(card+1);
				eat.realCards.push_back(card+2);

				eats.push_back(eat);

				return true;
			}
		}

		// 填充1子
		{
			// 前面填充
			if (card.getPoint() >= 2 && card.getPoint() <= 8 && __getCardCount(card+1) > 0)
			{
				std::map<Card, int> mapGostCount;
				__getGostAndCount(mapGostCount);

				// 找1个鬼
				for (std::map<Card, int>::iterator iter = mapGostCount.begin(); iter != mapGostCount.end(); ++iter)
				{
					Card theCardFillGost = iter->first;

					if (theCardFillGost == card || iter->second < 1)
						continue;

					__opCardCount(theCardFillGost, -1);
					__opCardCount(card, -1);
					__opCardCount(card+1, -1);
					
					bool bRet = __checkHu(eats);

					__opCardCount(theCardFillGost, 1);
					__opCardCount(card, 1);
					__opCardCount(card+1, 1);

					if (bRet)
					{
						SEatCard eat;
						eat.type = ectEat;
						eat.firstCard = card-1;
						eat.eatCard = card-1;

						// 真实牌型
						eat.realCards.push_back(theCardFillGost);
						eat.realCards.push_back(card);
						eat.realCards.push_back(card+1);
						
						eats.push_back(eat);

						return true;
					}
				}
			}

			// 中间填充
			if (card.getPoint() <= 7 && __getCardCount(card+2) > 0)
			{
				std::map<Card, int> mapGostCount;
				__getGostAndCount(mapGostCount);

				// 找1个鬼
				for (std::map<Card, int>::iterator iter = mapGostCount.begin(); iter != mapGostCount.end(); ++iter)
				{
					Card theCardFillGost = iter->first;

					if (theCardFillGost == card || iter->second < 1)
						continue;

					__opCardCount(card, -1);
					__opCardCount(theCardFillGost, -1);
					__opCardCount(card+2, -1);

					bool bRet = __checkHu(eats);

					__opCardCount(card, 1);
					__opCardCount(theCardFillGost, 1);
					__opCardCount(card+2, 1);

					if (bRet)
					{
						SEatCard eat;
						eat.type = ectEat;
						eat.firstCard = card;
						eat.eatCard = card;

						// 真实牌型
						eat.realCards.push_back(card);
						eat.realCards.push_back(theCardFillGost);
						eat.realCards.push_back(card+2);

						eats.push_back(eat);

						return true;
					}
				}
			}

			// 后面填充
			if (card.getPoint() <= 7 && __getCardCount(card+1) > 0)
			{
				std::map<Card, int> mapGostCount;
				__getGostAndCount(mapGostCount);

				// 找1个鬼
				for (std::map<Card, int>::iterator iter = mapGostCount.begin(); iter != mapGostCount.end(); ++iter)
				{
					Card theCardFillGost = iter->first;

					if (theCardFillGost == card || iter->second < 1)
						continue;

					__opCardCount(card, -1);
					__opCardCount(card+1, -1);
					__opCardCount(theCardFillGost, -1);

					bool bRet = __checkHu(eats);

					__opCardCount(card, 1);
					__opCardCount(card+1, 1);
					__opCardCount(theCardFillGost, 1);
					
					if (bRet)
					{
						SEatCard eat;
						eat.type = ectEat;
						eat.firstCard = card;
						eat.eatCard = card;

						// 真实牌型
						eat.realCards.push_back(card);
						eat.realCards.push_back(card+1);
						eat.realCards.push_back(theCardFillGost);
						
						eats.push_back(eat);

						return true;
					}
				}
			}
		}

		// 填充2子
		{
			// 前面填充
			if (card.getPoint() >= 3)
			{
				std::map<Card, int> mapGostCount;
				__getGostAndCount(mapGostCount);

				// 找2个同鬼
				for (std::map<Card, int>::iterator iter = mapGostCount.begin(); iter != mapGostCount.end(); ++iter)
				{
					Card theCardFillGost = iter->first;

					if (theCardFillGost == card || iter->second < 2)
						continue;

					__opCardCount(theCardFillGost, -1);
					__opCardCount(theCardFillGost, -1);
					__opCardCount(card, -1);

					bool bRet = __checkHu(eats);

					__opCardCount(theCardFillGost, 1);
					__opCardCount(theCardFillGost, 1);
					__opCardCount(card, 1);

					if (bRet)
					{
						SEatCard eat;
						eat.type = ectEat;
						eat.firstCard = card-2;
						eat.eatCard = card-2;

						// 真实牌型
						eat.realCards.push_back(theCardFillGost);
						eat.realCards.push_back(theCardFillGost);
						eat.realCards.push_back(card);

						eats.push_back(eat);

						return true;
					}
				}

				// 找2个异鬼
				for (std::map<Card, int>::iterator iter1 = mapGostCount.begin(); iter1 != mapGostCount.end(); ++iter1)
				{
					Card theCardFillGost1 = iter1->first;

					if (theCardFillGost1 == card || iter1->second < 1)
						continue;

					for (std::map<Card, int>::iterator iter2 = mapGostCount.begin(); iter2 != mapGostCount.end(); ++iter2)
					{
						Card theCardFillGost2 = iter2->first;

						if (theCardFillGost2 == card || theCardFillGost2 == theCardFillGost1 || iter2->second < 1)
							continue;

						__opCardCount(theCardFillGost1, -1);
						__opCardCount(theCardFillGost2, -1);
						__opCardCount(card, -1);

						bool bRet = __checkHu(eats);

						__opCardCount(theCardFillGost1, 1);
						__opCardCount(theCardFillGost2, 1);
						__opCardCount(card, 1);

						if (bRet)
						{
							SEatCard eat;
							eat.type = ectEat;
							eat.firstCard = card-2;
							eat.eatCard = card-2;

							// 真实牌型
							eat.realCards.push_back(theCardFillGost1);
							eat.realCards.push_back(theCardFillGost2);
							eat.realCards.push_back(card);

							eats.push_back(eat);

							return true;
						}
					}
				}
			}

			// 两边填充
			if (card.getPoint() >= 2 && card.getPoint() <= 8)
			{
				std::map<Card, int> mapGostCount;
				__getGostAndCount(mapGostCount);

				// 找2个同鬼
				for (std::map<Card, int>::iterator iter = mapGostCount.begin(); iter != mapGostCount.end(); ++iter)
				{
					Card theCardFillGost = iter->first;

					if (theCardFillGost == card || iter->second < 2)
						continue;

					__opCardCount(theCardFillGost, -1);
					__opCardCount(card, -1);
					__opCardCount(theCardFillGost, -1);

					bool bRet = __checkHu(eats);

					__opCardCount(theCardFillGost, 1);
					__opCardCount(card, 1);
					__opCardCount(theCardFillGost, 1);

					if (bRet)
					{
						SEatCard eat;
						eat.type = ectEat;
						eat.firstCard = card-1;
						eat.eatCard = card-1;

						// 真实牌型
						eat.realCards.push_back(theCardFillGost);
						eat.realCards.push_back(card);
						eat.realCards.push_back(theCardFillGost);

						eats.push_back(eat);

						return true;
					}
				}

				// 找2个异鬼
				for (std::map<Card, int>::iterator iter1 = mapGostCount.begin(); iter1 != mapGostCount.end(); ++iter1)
				{
					Card theCardFillGost1 = iter1->first;

					if (theCardFillGost1 == card || iter1->second < 1)
						continue;

					for (std::map<Card, int>::iterator iter2 = mapGostCount.begin(); iter2 != mapGostCount.end(); ++iter2)
					{
						Card theCardFillGost2 = iter2->first;

						if (theCardFillGost2 == card || theCardFillGost2 == theCardFillGost1 || iter2->second < 1)
							continue;

						__opCardCount(theCardFillGost1, -1);
						__opCardCount(card, -1);
						__opCardCount(theCardFillGost2, -1);

						bool bRet = __checkHu(eats);

						__opCardCount(theCardFillGost1, 1);
						__opCardCount(card, 1);
						__opCardCount(theCardFillGost2, 1);

						if (bRet)
						{
							SEatCard eat;
							eat.type = ectEat;
							eat.firstCard = card-1;
							eat.eatCard = card-1;

							// 真实牌型
							eat.realCards.push_back(theCardFillGost1);
							eat.realCards.push_back(card);
							eat.realCards.push_back(theCardFillGost2);

							eats.push_back(eat);

							return true;
						}
					}
				}
			}

			// 后面填充
			if (card.getPoint() <= 7)
			{
				std::map<Card, int> mapGostCount;
				__getGostAndCount(mapGostCount);

				// 找2个同鬼
				for (std::map<Card, int>::iterator iter = mapGostCount.begin(); iter != mapGostCount.end(); ++iter)
				{
					Card theCardFillGost = iter->first;

					if (theCardFillGost == card || iter->second < 2)
						continue;

					__opCardCount(card, -1);
					__opCardCount(theCardFillGost, -1);
					__opCardCount(theCardFillGost, -1);

					bool bRet = __checkHu(eats);

					__opCardCount(card, 1);
					__opCardCount(theCardFillGost, 1);
					__opCardCount(theCardFillGost, 1);

					if (bRet)
					{
						SEatCard eat;
						eat.type = ectEat;
						eat.firstCard = card;
						eat.eatCard = card;

						// 真实牌型
						eat.realCards.push_back(card);
						eat.realCards.push_back(theCardFillGost);
						eat.realCards.push_back(theCardFillGost);

						eats.push_back(eat);

						return true;
					}
				}

				// 找2个异鬼
				for (std::map<Card, int>::iterator iter1 = mapGostCount.begin(); iter1 != mapGostCount.end(); ++iter1)
				{
					Card theCardFillGost1 = iter1->first;

					if (theCardFillGost1 == card || iter1->second < 1)
						continue;

					for (std::map<Card, int>::iterator iter2 = mapGostCount.begin(); iter2 != mapGostCount.end(); ++iter2)
					{
						Card theCardFillGost2 = iter2->first;

						if (theCardFillGost2 == card || theCardFillGost2 == theCardFillGost1 || iter2->second < 1)
							continue;

						__opCardCount(card, -1);
						__opCardCount(theCardFillGost1, -1);
						__opCardCount(theCardFillGost2, -1);

						bool bRet = __checkHu(eats);

						__opCardCount(card, 1);
						__opCardCount(theCardFillGost1, 1);
						__opCardCount(theCardFillGost2, 1);

						if (bRet)
						{
							SEatCard eat;
							eat.type = ectEat;
							eat.firstCard = card;
							eat.eatCard = card;

							// 真实牌型
							eat.realCards.push_back(card);
							eat.realCards.push_back(theCardFillGost1);
							eat.realCards.push_back(theCardFillGost2);

							eats.push_back(eat);

							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

// 检测特殊胡牌类型

bool CCheck::__is7Duizi(EATCARDS_t& eats, int& n4cnt)
{
	if (m_handCards.size() != 14)
		return false;

	if (!__check7Duizi(eats))
		return false;

	n4cnt = 0;
	for (EATCARDS_t::iterator iter = eats.begin(); iter != eats.end(); ++iter)
	{
		if (iter->type == ectGang)
			++n4cnt;
	}

	return true;
}

bool CCheck::__check7Duizi(EATCARDS_t& eats)
{
	Card card = __remainCard();

	if (!card.isValid())
		return true;

	int theCardCount = __getCardCount(card);

	// 不填充
	if (theCardCount == 2)
	{
		__opCardCount(card, -1);
		__opCardCount(card, -1);

		bool bRet = __check7Duizi(eats);

		__opCardCount(card, 1);
		__opCardCount(card, 1);

		if (bRet)
		{
			SEatCard eat;
			eat.type = ectDui;
			eat.firstCard = card;
			eat.eatCard = card;

			// 真实牌型
			eat.realCards.push_back(card);
			eat.realCards.push_back(card);

			eats.push_back(eat);

			return true;
		}
	}
	
	if (theCardCount == 4)
	{
		__opCardCount(card, -1);
		__opCardCount(card, -1);
		__opCardCount(card, -1);
		__opCardCount(card, -1);

		bool bRet = __check7Duizi(eats);

		__opCardCount(card, 1);
		__opCardCount(card, 1);
		__opCardCount(card, 1);
		__opCardCount(card, 1);

		if (bRet)
		{
			SEatCard eat;
			eat.type = ectGang;
			eat.firstCard = card;
			eat.eatCard = card;

			// 真实牌型
			eat.realCards.push_back(card);
			eat.realCards.push_back(card);
			eat.realCards.push_back(card);
			eat.realCards.push_back(card);

			eats.push_back(eat);

			return true;
		}
	}

	// 填充1子
	if (theCardCount == 1)
	{
		std::map<Card, int> mapGostCount;
		__getGostAndCount(mapGostCount);

		// 找1个鬼
		for (std::map<Card, int>::iterator iter = mapGostCount.begin(); iter != mapGostCount.end(); ++iter)
		{
			Card theCardFillGost = iter->first;

			if (theCardFillGost == card || iter->second < 1)
				continue;

			__opCardCount(card, -1);
			__opCardCount(theCardFillGost, -1);

			bool bRet = __check7Duizi(eats);

			__opCardCount(card, 1);
			__opCardCount(theCardFillGost, 1);

			if (bRet)
			{
				SEatCard eat;
				eat.type = ectDui;
				eat.firstCard = card;
				eat.eatCard = card;

				// 真实牌型
				eat.realCards.push_back(card);
				eat.realCards.push_back(theCardFillGost);

				eats.push_back(eat);

				return true;
			}
		}
	}

	if (theCardCount == 3)
	{
		std::map<Card, int> mapGostCount;
		__getGostAndCount(mapGostCount);

		// 找1个鬼
		for (std::map<Card, int>::iterator iter = mapGostCount.begin(); iter != mapGostCount.end(); ++iter)
		{
			Card theCardFillGost = iter->first;

			if (theCardFillGost == card || iter->second < 1)
				continue;

			__opCardCount(card, -1);
			__opCardCount(card, -1);
			__opCardCount(card, -1);
			__opCardCount(theCardFillGost, -1);

			bool bRet = __check7Duizi(eats);

			__opCardCount(card, 1);
			__opCardCount(card, 1);
			__opCardCount(card, 1);
			__opCardCount(theCardFillGost, 1);

			if (bRet)
			{
				SEatCard eat;
				eat.type = ectGang;
				eat.firstCard = card;
				eat.eatCard = card;

				// 真实牌型
				eat.realCards.push_back(card);
				eat.realCards.push_back(card);
				eat.realCards.push_back(card);
				eat.realCards.push_back(theCardFillGost);

				eats.push_back(eat);

				return true;
			}
		}
	}

	return false;
}

bool CCheck::__isPengpengHu(EATCARDS_t& eats)
{
	int jiangCount = 0;

	// 判断手上的牌
	for (MAP_CARD_COUNT_t::iterator iter = m_mapCardCount.begin(); iter != m_mapCardCount.end(); ++iter)
	{
		if (iter->second == 2)
		{
			if (jiangCount > 0)
				return false;

			SEatCard eat;
			eat.type = ectDui;
			eat.firstCard = iter->first;
			eat.eatCard = iter->first;
			eats.push_back(eat);

			jiangCount++;
		}
		else if (iter->second == 3)
		{
			SEatCard eat;
			eat.type = ectPeng;
			eat.firstCard = iter->first;
			eat.eatCard = iter->first;
			eats.push_back(eat);
		}
		else
			return false;
	}

	// 必须有将
	if (jiangCount != 1)
		return false;

	// 判断吃出去的牌
	for (EATCARDS_t::iterator iter = m_eatCards.begin(); iter != m_eatCards.end(); ++iter)
	{
		if (iter->type != ectPeng && iter->type != ectGang && iter->type != ectAnGang)
			return false;
	}

	return true;
}

bool CCheck::__isQuanqiuren(EATCARDS_t& eats)
{
	// 判断手上的牌
	if (m_handCards.size() == 2 && m_handCards[0] == m_handCards[1])
	{
		SEatCard eat;
		eat.type = ectDui;
		eat.firstCard = m_handCards[0];
		eat.eatCard = m_handCards[1];
		eats.push_back(eat);

		return true;
	}

	return false;
}

bool CCheck::__isQingyise()
{
	int colorFirst = -1;

	// 判断手上的牌
	for (MAP_CARD_COUNT_t::iterator iter = m_mapCardCount.begin(); iter != m_mapCardCount.end(); ++iter)
	{
		if (iter->second == 0)
			continue;

		if (colorFirst == -1)
		{
			colorFirst = iter->first.getColor();
			continue;
		}

		if (iter->first.getColor() != colorFirst)
			return false;
	}

	// 判断吃出去的牌
	for (EATCARDS_t::iterator iter = m_eatCards.begin(); iter != m_eatCards.end(); ++iter)
	{
		if (iter->firstCard.getColor() != colorFirst)
			return false;
	}

	return true;
}

// 检测鬼牌
bool CCheck::__isGost(const Card& card)
{
	for (CARDS_t::iterator iter = m_local.m_cardsGost.begin(); iter != m_local.m_cardsGost.end(); ++iter)
	{
		if (*iter == card)
			return true;
	}

	return false;
}

// 取鬼牌及其数量
void CCheck::__getGostAndCount(std::map<Card, int>& mapGostCount)
{
	for (CARDS_t::iterator iter = m_local.m_cardsGost.begin(); iter != m_local.m_cardsGost.end(); ++iter)
	{
		mapGostCount[*iter] = __getCardCount(*iter);
	}
}
