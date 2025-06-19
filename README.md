# Linux (WSL2)への移植版です

## ビルド方法
```
mkdir build
cd build

cmake ..
```

以下の行を mjaigame/CMakeLists.txt に追加
```
target_compile_definitions(mjai_ai PRIVATE RELEASE_BUILD)
```
最後に以下を実行

```
make
```
---
・mjai-occamはmjaiのクライアントです

・ホームページ http://cmj3.web.fc2.com/tenhou/tenhoutest.html

・ver0.524

・XCode8.2で開発しています

・実行ファイルの下に opponenthoraestimator/exeと同じ階層に置くファイル 以下のファイルを配置する必要があります

・MJAIGame/MJAIGame.xcodeprojがメインのプロジェクトファイルです

・個人制作で特に配布を意識して作られたものではないのでソースとかは汚いし説明も少ないです（とりあえずバックアップ目的で公開したものです）。

・もし使いたかったらソースとか読み解いてご自由に。
