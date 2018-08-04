//============================================================================
// Name        : Hands.cpp
// Author      : kdjie
// Version     : 1.0
// Copyright   : @2015
// Description : 14166097@qq.com
//============================================================================

#include "Hands.h"

// 清除手上的牌
void CHands::clearHandCards()
{
	m_handCards.clear();
}

// 增加手上的牌（发牌/摸牌）
void CHands::addHandCards(const CARDS_t& cards)
{
	std::copy(cards.begin(), cards.end(), std::back_inserter(m_handCards));

	__sortHandCards();
}
void CHands::addHandCard(const Card& card)
{
	m_handCards.push_back(card);

	__sortHandCards();
}

// 减小手上的牌（打牌/吃牌）
void CHands::delHandCard(const Card& card)
{
	CARDS_t::iterator iter = std::find(m_handCards.begin(), m_handCards.end(), card);
	if (iter != m_handCards.end())
	{
		m_handCards.erase(iter);
	}
}

// 获取手上的牌
const CARDS_t& CHands::getHandCards() const
{
	return m_handCards;
}

// 清除吃出去的牌
void CHands::clearEatCards()
{
	m_eatCards.clear();
}

// 增加吃出去的牌
void CHands::addEatCard(const SEatCard& eat)
{
	m_eatCards.push_back(eat);
}

// 删除吃出去的牌
void CHands::delEatCard(const SEatCard& eat)
{
	for (EATCARDS_t::iterator iter = m_eatCards.begin(); iter != m_eatCards.end(); ++iter)
	{
		if (iter->type == eat.type && iter->firstCard == eat.firstCard && iter->eatCard == eat.eatCard)
		{
			m_eatCards.erase(iter);
			break;
		}
	}
}

// 获取吃出去的牌
const EATCARDS_t& CHands::getEatCards() const
{
	return m_eatCards;
}

// 排序手上的牌
void CHands::__sortHandCards()
{
	std::sort(m_handCards.begin(), m_handCards.end(), Card_Lesser);
}

// 清除打出的牌
void CHands::clearOutCards()
{
	m_outCards.clear();
}

// 增加打出的牌
void CHands::pushOutCard(const Card& card)
{
	m_outCards.push_back(card);
}

// 弹出打出的牌
void CHands::popOutCard()
{
	if (m_outCards.empty())
		return;

	m_outCards.pop_back();
}

// 获取历史出牌
const CARDS_t& CHands::getOutCards() const
{
	return m_outCards;
}
