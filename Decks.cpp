//============================================================================
// Name        : Decks.cpp
// Author      : kdjie
// Version     : 1.0
// Copyright   : @2015
// Description : 14166097@qq.com
//============================================================================

#include "Decks.h"


void CDecks::setNoFeng(bool b)
{
	m_decksOptions.bNoFeng = b;
}

void CDecks::clear()
{
	m_cards.clear();
}

void CDecks::fillCards()
{
	clear();

	// 填普通牌
	for (int c = MJ_COLOR_WZ; c <= MJ_COLOR_TZ; ++c)
	{
		for (int p = 1; p <= 9; ++p)
		{
			for (int i = 0; i < 4; ++i)
			{
				m_cards.push_back( Card(c, p) );
			}
		}
	}

	// 填风牌
	if (!m_decksOptions.bNoFeng)
	{
		for (int i = 0; i < 4; ++i)
		{
			m_cards.push_back( Card(MJ_COLOR_FZ, FZ_POINT_HONGZHONG) );
			m_cards.push_back( Card(MJ_COLOR_FZ, FZ_POINT_DONG) );
			m_cards.push_back( Card(MJ_COLOR_FZ, FZ_POINT_NAN) );
			m_cards.push_back( Card(MJ_COLOR_FZ, FZ_POINT_XI) );
			m_cards.push_back( Card(MJ_COLOR_FZ, FZ_POINT_BEI) );
			m_cards.push_back( Card(MJ_COLOR_FZ, FZ_POINT_FACAI) );
			m_cards.push_back( Card(MJ_COLOR_FZ, FZ_POINT_BAIBAN) );
		}
	}

	// 洗牌
	srand(unsigned(time(NULL)));
	std::random_shuffle(m_cards.begin(), m_cards.end());
}

bool CDecks::fetchCard(Card& card)
{
	if (m_cards.empty())
		return false;

	card = *m_cards.begin();
	m_cards.erase(m_cards.begin());

	return true;
}

int CDecks::fetchCards(CARDS_t& cards, int count)
{
	for (int i = 0; i < count; ++i)
	{
		Card card;
		if (!fetchCard(card))
			break;

		cards.push_back(card);
	}

	return cards.size();
}

void CDecks::pushCard(const Card& card)
{
	if (!card.isValid())
		return;

	m_cards.push_back(card);
}

int	CDecks::cardCount() const
{
	return m_cards.size();
}

const CARDS_t& CDecks::getCards() const
{
	return m_cards;
}

CARDS_t CDecks::randomCards(int count, bool bNoFeng)
{
	if (count <= 0)
		return CARDS_t();

	CARDS_t deckCards;

	// 填普通牌
	for (int c = MJ_COLOR_WZ; c <= MJ_COLOR_TZ; ++c)
	{
		for (int p = 1; p <= 9; ++p)
		{
			for (int i = 0; i < 4; ++i)
			{
				deckCards.push_back( Card(c, p) );
			}
		}
	}

	// 填风牌
	if (!bNoFeng)
	{
		for (int i = 0; i < 4; ++i)
		{
			deckCards.push_back( Card(MJ_COLOR_FZ, FZ_POINT_HONGZHONG) );
			deckCards.push_back( Card(MJ_COLOR_FZ, FZ_POINT_DONG) );
			deckCards.push_back( Card(MJ_COLOR_FZ, FZ_POINT_NAN) );
			deckCards.push_back( Card(MJ_COLOR_FZ, FZ_POINT_XI) );
			deckCards.push_back( Card(MJ_COLOR_FZ, FZ_POINT_BEI) );
			deckCards.push_back( Card(MJ_COLOR_FZ, FZ_POINT_FACAI) );
			deckCards.push_back( Card(MJ_COLOR_FZ, FZ_POINT_BAIBAN) );
		}
	}

	// 洗牌
	srand(unsigned(time(NULL)));
	std::random_shuffle(deckCards.begin(), deckCards.end());

	CARDS_t cards;
	std::copy(deckCards.begin(), deckCards.begin()+count, std::back_inserter(cards));

	return cards;
}

void CDecks::fillCards(const CARDS_t& cards)
{
	m_cards = cards;
}
