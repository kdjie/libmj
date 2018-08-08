# libmj

这是一个经典的广东麻将牌库算法，下面提供的用法，是我从工程中摘录过来的，难免有不全或错误，希望读者能有个大概理解。

需要引入头文件：
#include "libmj/Decks.h"

#include "libmj/Hands.h"

#include "libmj/Check.h"

初使化成员或者全局对象：
CDecks m_decks; // 底牌
map<int, CHands> m_mapSeatHands; // 每个座位的手牌

洗牌：
m_decks.setZhongGost( true ); // 设置开放鬼牌
m_decks.setBlankGost( true ); // 设置允许白板为鬼牌
m_decks.setNoFeng( false );  // 设置无风牌
m_decks.fillCards(); // 洗牌动作

初使发牌，放到手牌中：
CARDS_t cards;
m_decks.fetchCards(cards, 14);
m_mapSeatHands[0].addHandCards(cards);
m_decks.fetchCards(cards, 13);
m_mapSeatHands[1].addHandCards(cards);
m_decks.fetchCards(cards, 13);
m_mapSeatHands[2].addHandCards(cards);
m_decks.fetchCards(cards, 13);
m_mapSeatHands[3].addHandCards(cards);

后续拿牌，放到手牌中：
Card card;
m_decks.fetchCard(card);
m_mapSeatHands[1].addHandCards(cards);

出牌，将手中的牌移动到出牌列表：
m_mapSeatHands[1].delHandCard(card);
m_mapSeatHands[1].pushOutCard(card);

胡牌调用：
CCheck check(m_mapSeatHands[1]);
check.setSupport7Duizi( true );
bool bHu = false, bBu = false;
EATCARDS_t eatHuCards, eatBuCards;
bHu = check.canHu(eatHuCards);
CARDS_t cardsPassBu;
bBu = check.canBu(eatBuCards, cardsPassBu);
