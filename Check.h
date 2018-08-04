//============================================================================
// Name        : Check.h
// Author      : kdjie
// Version     : 1.0
// Copyright   : @2015
// Description : 14166097@qq.com
//============================================================================

#pragma once

#include "Card.h"
#include "Hands.h"
#include <map>
#include <set>

// 胡牌类型
enum EHuType
{
	E_HU_XIAO = 1,		// 小胡
	E_HU_7DUIZI,		// 7对子
	E_HU_H7DUIZI,		// 豪华7对子
	E_HU_2H7DUIZI,		// 2豪华7对子
	E_HU_3H7DUIZI,		// 3豪华7对子
	E_HU_PENGPENG,		// 碰碰胡（大对胡）
	E_HU_QINGYISE,		// 清一色
	E_HU_QUANQIUREN,	// 全求人（单飞）
	E_HU_13LAN,			// 13烂
	E_HU_7X13LAN,		// 7星13烂

	// 胡牌方式
	E_HU_PAO = 100,		// 接炮
	E_HU_ZIMO,			// 自摸
	E_HU_TIANHU,		// 天胡
	E_HU_DIHU,			// 地胡
	E_HU_QIANGBUHU,		// 抢补胡
};

class CCheck
{
public:
	CCheck() {}
	virtual ~CCheck() {}

	CCheck(const CHands& hands);

	// 设置鬼牌
	void setGostCard(const Card& card);
	void setGostCard(const CARDS_t& cards);

	// 设置是否支持相关胡牌类型
	void setSupport7Duizi(bool b);
	void setSupportPengpeng(bool b);
	void setSupportQingyise(bool b);
	void setSupportQuanqiuren(bool b);

	// 设置手上的牌
	void setHandCards(const CARDS_t& cards);
	// 变更手上的牌
	void addHandCard(const Card& card);
	void delHandCard(const Card& card);

	// 设置吃出去的牌
	void setEatCards(const EATCARDS_t& eats);

	// 检测能否吃牌，返回可选牌型
	bool canEat(const Card& card, EATCARDS_t& eats);

	// 检测能否碰牌
	bool canPeng(const Card& card);

	// 检测能否补牌
	bool canBu(const Card& card);
	bool canBu(EATCARDS_t& eats, const CARDS_t& cardsPassPeng);

	// 检测能否胡牌
	bool canHu(const Card& card, EATCARDS_t& eats);
	bool canHu(EATCARDS_t& eats);

	// 获取胡牌类型
	std::set<int> getHuTypes();

	// 检测能否听牌
	bool canTing();
	bool canTing(CARDS_t& cards);

private:

	// 统计每种牌的张数
	void __calcCardCount();

	// 获取每种牌的张数
	int __getCardCount(const Card& card);

	// 操作每种牌增减
	void __opCardCount(const Card& card, int cnt);

	Card __remainCard();
	bool __checkHu(EATCARDS_t& eats);

	// 检测特殊胡牌类型
	bool __is7Duizi(EATCARDS_t& eats, int& n4cnt);
	bool __check7Duizi(EATCARDS_t& eats);

	bool __isPengpengHu(EATCARDS_t& eats);
	bool __isQuanqiuren(EATCARDS_t& eats);
	bool __isQingyise();

	// 检测鬼牌
	bool __isGost(const Card& card);
	// 取鬼牌及其数量
	void __getGostAndCount(std::map<Card, int>& mapGostCount);

private:
	typedef std::map<Card, int> MAP_CARD_COUNT_t;

	CARDS_t m_handCards;
	EATCARDS_t m_eatCards;
	MAP_CARD_COUNT_t m_mapCardCount;

	struct SLocalVal
	{
		CARDS_t m_cardsGost;

		bool m_bSupport7Duizi;
		bool m_bSupportPengpeng;
		bool m_bSupportQingyise;
		bool m_bSupportQuanqiuren;

		bool m_bDui;
		std::set<int> m_setHuType;

		SLocalVal()
		{
			m_bSupport7Duizi = false;
			m_bSupportPengpeng = false;
			m_bSupportQingyise = false;
			m_bSupportQuanqiuren = false;

			reset();
		}

		void reset()
		{
			m_bDui = false;
			m_setHuType.clear();
		}
	} m_local;
};
