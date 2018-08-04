//============================================================================
// Name        : Hands.h
// Author      : kdjie
// Version     : 1.0
// Copyright   : @2015
// Description : 14166097@qq.com
//============================================================================

#pragma once

#include "Card.h"

// 吃碰杠补牌型（也用于其他牌型判断）
enum EEatType
{
	ectEat = 1,	// 吃牌
	ectPeng,	// 碰牌
	ectGang,	// 明杠
	ectAnGang,	// 暗杠
	ectDui,		// 对子
	ectSingle,	// 单牌

	ectNone = -1
};

// 吃碰杠的牌
struct SEatCard
{
	EEatType type;

	Card firstCard;
	Card eatCard;

	CARDS_t realCards; // 真实牌型

	SEatCard()
	{
		type = ectNone;
	}
};

typedef std::vector<SEatCard> EATCARDS_t;

class CHands
{
public:
	CHands() {}
	virtual ~CHands() {}

	// 清除手上的牌
	void clearHandCards();
	// 增加手上的牌（发牌/摸牌）
	void addHandCards(const CARDS_t& cards);
	void addHandCard(const Card& card);
	// 减小手上的牌（打牌/吃牌）
	void delHandCard(const Card& card);
	// 获取手上的牌
	const CARDS_t& getHandCards() const;

	// 清除吃出去的牌
	void clearEatCards();
	// 增加吃出去的牌
	void addEatCard(const SEatCard& eat);
	// 删除吃出去的牌
	void delEatCard(const SEatCard& eat);
	// 获取吃出去的牌
	const EATCARDS_t& getEatCards() const;

	// 清除打出的牌
	void clearOutCards();
	// 增加打出的牌
	void pushOutCard(const Card& card);
	// 弹出打出的牌
	void popOutCard();
	// 获取历史出牌
	const CARDS_t& getOutCards() const;

private:

	// 排序手上的牌
	void __sortHandCards();

private:
	// 手上的牌
	CARDS_t m_handCards;

	// 吃出去的牌
	EATCARDS_t m_eatCards;

	// 打出去的牌
	CARDS_t m_outCards;
};
