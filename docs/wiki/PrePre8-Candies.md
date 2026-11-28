# Candies in the Tomba! 1 builds

[All PrePre 8 events](PrePre8-Events.md) · from [PREPRE8_EVENTS.md](../PREPRE8_EVENTS.md)


Retail item names are PAL English (D0SPSUB) and JP retail; JP descriptions are `DSPSUB.BIN` block 0 entry id + 4.
Use effects are the item-use switch, `src/d0spsub/func_800E89F8.c` (PAL) and `func_800EA2BC_slps01144.c` (JP, the same
cases). "Given" means an `addItemToInventory` with that id exists (CUT_CONTENT.md: 110-114 are never given).

| Id | JP name | English | Obtainable | Use effect / purpose | JP description |
|---|---|---|---|---|---|
| 110 (0x6E) | 赤のドロップ | Red Candy | no | none (no use case) | たべてみると わかるハズ ("you'll know when you eat it") |
| 111 (0x6F) | 青のドロップ | Blue Candy | no | +4 vitality, consumed | same |
| 112 (0x70) | 緑のドロップ | Green Candy | no | +8 vitality, consumed (the same code as the Lunch Box) | same |
| 113 (0x71) | 黒のドロップ | Black Candy | no | full vitality, consumed (the same code as the Large Lunch Box) | same |
| 114 (0x72) | 銀のドロップ | Silver Candy | no | none | same |
| 115 (0x73) | 金のドロップ | Gold Candy | **yes** | made at witch Mizuno's cauldron (event 120 Let's Make Candy!, X018 `witchMizunoCandyRecipeTalk`): placing the six ingredients completes 120 and gives one | ５大アイテムのひとつ ("one of the five great items", i.e. The 5 Golden Items, event 127) |

The recipe (MFNT180 3/15-22, PAL text "To make candy you need..."; JP 「ドロップをつくるには つぎの材料が いるんだからね！！」):

| Ingredient | Id | JP | Notes |
|---|---|---|---|
| Biting Plant Flower | 8 | かみつき草の花 | handing it over completes event 169 (PAL/USA); JP retail and the beta complete 169 when the item is *used* (notes/secrets.md) |
| Silver Powder | 102 (0x66) | 銀の粉 | |
| Molasses | 103 (0x67) | 黒みつ | |
| Kokka Claw | 104 (0x68) | コッカのツメ | |
| Butamushi Thorn | 105 (0x69) | ブタムシのトゲ | |
| Needlegator Teeth | 106 (0x6A) | ハリゲーターのキバ | "the secret ingredient" |

Differences between builds:
- **JP retail** = PAL for the candies: same ids, the same three use cases, the same six ingredients in the dialogue.
  But the JP hint for event 120 says 「３つのアイテムを調合して 魔法の ドロップをつくろう！」 ("combine **three**
  items to make a magic candy"): an earlier, smaller recipe survives in the hint.
- **PrePre 8** has no candy at all: its item table has six names (ヒヨコ, カエル, ドワーフの迷子, バナナ, たつまき,
  花の〓) and the id of Let's Make Candy! (120) was then 121 トビウオ アクロバット ("flying fish acrobatics"). The
  witch is already there (115 魔女の探し物 = retail 114 What the Witch Lost...).
- **Beta 980203, USA, the other demos**: not checked for candies on this pass (the beta's item-use switch is the
  retail one per notes/secrets.md, apart from event 169).
- The four coloured candies share their colours with PrePre 8's four fortune tellers (赤 45, 緑 90, 青 129, 黒 147),
  and silver/gold with the medals. This is an observation, not evidence of a link.
- Tomba! 2 has "Collect Ice Candy" and an Ice Candy item ("a carnival favorite"); its lunch boxes restore 4 / 8 / 15
  vitality, the same 4 and 8 steps as T1's blue and green candies.
