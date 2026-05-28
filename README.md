# SampleIME
Text Services Frameworkを使ったIMEのサンプルです。日本語IMEとして選択することができますが，文字の入力はできません。

## ビルド
お好みのCコンパイラを使って，`Advapi32.def`, `msctf.def`, `Ole32.def`, `src/*.c`をまとめてDLLとしてコンパイルしてください。（Windows APIのヘッダファイルが必要です。）

`msctf_guid.c`内のGUIDが再定義になる場合は`msctf_guid.c`を削除してください。

## インストール
ビルドしたDLLをregsvr32コマンドで登録してください。
