//============================================================================
// Name        : Decks.h
// Author      : kdjie
// Version     : 1.0
// Copyright   : @2015
// Description : 14166097@qq.com
//============================================================================

#pragma once

#include "Card.h"

struct SDecksOptions
{
	bool bNoFeng;

	SDecksOptions()
		: bNoFeng(false)
	{
	}
};

class CDecks
{
public:
	CDecks() {}
	virtual ~CDecks() {}

	void setNoFeng(bool b);

	void clear();
	void fillCards();
	bool fetchCard(Card& card);
	int fetchCards(CARDS_t& cards, int count);
	void pushCard(const Card& card);

	int cardCount() const;
	const CARDS_t& getCards() const;

	static CARDS_t randomCards(int count, bool bNoFeng);

	void fillCards(const CARDS_t& cards);

private:
	CARDS_t m_cards;

	SDecksOptions m_decksOptions;
};
