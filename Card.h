//============================================================================
// Name        : Card.h
// Author      : kdjie
// Version     : 1.0
// Copyright   : @2015
// Description : 14166097@qq.com
//============================================================================

#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

// 麻将花色
#define MJ_COLOR_WZ						1			// 万字
#define MJ_COLOR_BZ						2			// 饼字
#define MJ_COLOR_TZ						3			// 条字
#define MJ_COLOR_FZ						4			// 东南西北红中白板发财等

// 风字的点数
#define FZ_POINT_HONGZHONG				1			// 红中
#define FZ_POINT_DONG					2			// 东
#define FZ_POINT_NAN					3			// 南
#define FZ_POINT_XI					4			// 西
#define FZ_POINT_BEI					5			// 北
#define FZ_POINT_FACAI					6			// 发财
#define FZ_POINT_BAIBAN				7			// 白板

class Card
{
public:
	Card()
		: m_val(-1)
	{
	}

	Card(int val)
		: m_val(-1)
	{
		if ( __isValidCard(val) )
		{
			m_val = val;
		}
	}

	Card(int col, int po)
		: m_val(-1)
	{
		int val = col*10 + po;
		if ( __isValidCard(val) )
		{
			m_val = val;
		}
	}

	Card(const Card& card)
	{
		m_val = card.m_val;
	}

	bool isValid() const
	{
		return (m_val != -1);
	}

	void setValForce(int val)
	{
		m_val = val;
	}

	int getVal() const
	{
		return m_val;
	}

	int getColor() const
	{
		if (m_val == -1)
			return -1;

		return m_val/10;
	}

	int getPoint() const
	{
		if (m_val == -1)
			return -1;

		return m_val%10;
	}

	Card& operator = (const Card& card)
	{
		m_val = card.m_val;
		return *this;
	}

	bool operator == (const Card& card) const
	{
		return (m_val == card.m_val);
	}

	bool operator != (const Card& card) const
	{
		return (m_val != card.m_val);
	}

	bool operator < (const Card& card) const
	{
		return (m_val < card.m_val);
	}

	Card operator + (int n) const
	{
		return Card( m_val + n );
	}

	Card operator - (int n) const
	{
		return Card( m_val - n );
	}

private:
	
	bool __isValidCard(int val)
	{
		if (__isNormalCard(val))
			return true;

		if (__isFengCard(val))
			return true;

		return false;
	}

	bool __isNormalCard(int val)
	{
		int col = val/10;
		int po = val%10;

		return (col >= MJ_COLOR_WZ && col <= MJ_COLOR_TZ && po >= 1 && po <= 9);
	}

	bool __isFengCard(int val)
	{
		int col = val/10;
		int po = val%10;

		return (col == MJ_COLOR_FZ && po >= FZ_POINT_HONGZHONG && po <= FZ_POINT_BAIBAN);
	}

private:
	int m_val;
};

inline bool Card_Lesser(const Card& a, const Card& b)
{
	return a < b;
}

typedef std::vector<Card> CARDS_t;

inline std::string toCardsString(const CARDS_t& cards)
{
	std::stringstream ss;
	for (CARDS_t::const_iterator c_iter = cards.begin(); c_iter != cards.end(); ++c_iter)
	{
		ss << c_iter->getVal() << ",";
	}
	return ss.str();
}
