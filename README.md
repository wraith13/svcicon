# svcicon

Windows のサービスの稼働状態をタスクバーの通知領域で監視及び開始/終了などの操作をする為のツールです。

## 使用方法

下のような感じで引数を与えてください。(全て省略すると usage が表示されます。)

```cmd
svcicon.exe [サービス名[@コンピュータ名] [アイコンファイル名 [アイコンインデックス値]]]
```

### 自動起動について

Windows のログイン時に自動的に起動するような仕組みは作ってないので必要に応じて [ファイル名を指定して実行] ( [田]+[R] ) から `shell:startup` を開いてショートカットを登録してください。

### 使用例

```cmd
svcicon.exe w3svc
```

World Wide Web 発行サービス(w3svc) の稼働状態の監視と操作ができるアイコンがタスクバーの通知領域に表示されます。通知領域のアイコンには default のモノを使用。

> [コンピューターの管理] → [サービスとアプリケーション] → [サービス] で表示されるサービスの一覧から目的のサービスを選択し [プロパティ] を表示すると svcicon の引数で指定できる [サービス名] を確認できます。

## ビルド方法

### 前準備

#### [Visual Studio](https://www.visualstudio.com/)

Visual Studio のインストールは C++ コンパイラと Windows SDK が含まれるようにしてください。
Visual Studio 2017 より新しい Visual Studio や通常と異なるパスにインストールした場合などは `.\source\solomon\conf\config.%COMPUTERNAME%.cmd` を作成し

```cmd
@SET VCVARSALL_PATH=C:\Program Files\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat
```

といった感じで vcvarsall.bat のパスを指定してください。

> 非推奨となりますが、その他の C++ コンパイラでもビルドすることは可能です。 Borland C++ Compiler でのビルドは[こちら](#solomon-を使わない-borland-c-compiler-でのビルド非推奨)を参照してください。 mingw でのコンパイルは .\source\Makefile を使ってビルドしてください。( [@mattn](https://github.com/mattn) さんが [neticon](https://github.com/wraith13/neticon) 用に対応してくださった[ヤツ](https://github.com/wraith13/neticon/blob/master/source/Makefile)を丸パクりしただけで動作確認してません。 )

#### [solomon](https://github.com/wraith13/solomon)

solomon についてはこのプロジェクトのローカルコピーのパスが `C:\github\wraith13\svcicon` である場合に  `C:\github\wraith13\solomon` のように隣にローカルコピーを用意する分には設定の必要はありませんがそれ以外のパスに用意した場合は `.\source\solomon\conf\config.%COMPUTERNAME%.cmd` を作成し

```cmd
@SET SOLOMON_MAIN_CMD=C:\github\wraith13\solomon\cmd\main.cmd
```

といった感じで solomon の main.cmd のパスを指定してください。

> 非推奨となりますが、 solomon がなくともビルドすることは可能です。詳細は[こちら](#solomon-を使わない-vc-でのビルド非推奨)を参照してください。

### ビルドの実行

コマンド プロンプトから次の cmd を実行します。

```cmd
.\source\solomon\build.cmd
```

正常にビルドが完了すれば コマンド プロンプト の文字が緑色になり `.\snapshot\result\` にビルドされたバイナリがビルドのタイプ別に出力されます。

> この cmd に限らずこのプロジェクトの cmd の実行は削除対象となるディレクトリなどを除けば基本的にどのカレントディレクトリからの実行でも構いません。

次の cmd を実行しておくと `.\source\` ディレクトリ下の変更を検知しファイルが更新される度に自動的にビルドが実行されます。

```cmd
.\source\solomon\auto.build.cmd
```

### リリース用パッケージの作成

事前にビルドを行った上で、コマンド プロンプトから次の cmd を実行すると `.\release\` ディレクトリにリリース用パッケージが作成されます。

```cmd
.\make.release.package.cmd
```

### solomon を使わない VC でのビルド(非推奨)

最初に `.\source\solomon\build\subcmd\build.cmd` の内容を次のように書き換えます。

```cmd
@REM
@REM build
@REM

@SETLOCAL
@CALL "%CALL_VCVARSALL_CMD%" %VCVARSALL_ARG% >NUL

@REM リソースコンパイル
rc %SVCICON_MACRO% .\svcicon.rc

@REM 通常コンパイル
cl ".\svcicon.cpp" %VC_CL_ARG% %SVCICON_MACRO% /EHsc /MP /W4 /Fesvcicon.exe /link %VCLINKER_ARG% svcicon.res

@ENDLOCAL
```

上の書き換えが済んだ上で、次のいずれかの cmd を実行してください。

```cmd
".\source\solomon\build\debug x86 ansi.cmd"
```

```cmd
".\source\solomon\build\release x64 unicode.cmd"
```

```cmd
".\source\solomon\build\release x86 unicode.cmd"
```

ビルドに成功すると `.\source\svcicon.exe` が作成されます。

> 必要応じて `.\source\solomon\build\subcmd\increment.version.cmd` を実行するか、あるいは手作業で `VERSION.cmd` 内のビルド番号をインクリメントしてください。 solomon でのビルジ時は全てのビルドに成功すると自動的に `.\source\solomon\build\subcmd\increment.version.cmd` が呼び出されます。

### solomon を使わない Borland C++ Compiler でのビルド(非推奨)

`.\source\BUILD.h` の内容を次のように書き換えます。( BUILD の定義は区別がつけば基本的になんでも構いません。 )

```c++
#define BUILD Borland Build
```

`.\source\VERSION.h` の内容を手業で編集しバージョン情報を設定してください。

以上の前準備が済めば次のコマンドでビルドできます。( Borland C++ Compiler のパス設定やライブラリパス等々の設定が済んでる前提 )

```cmd
bcc32 -tWM -O1 -c svcicon.cpp
brcc32 svcicon.rc
ilink32 -aa svcicon.obj, svcicon.exe, , c0w32.obj import32.lib cw32mt.lib, , svcicon.res
```

## ファイル/ディレクトリ構成

### .\readme.md

このファイルです。

### .\LICENSE_1_0.txt

このソフトウェアで採用しているライセンス

### .\make.release.package.cmd

リリース用パッケージ作成バッチです。
このバッチが行うのはリリース用パッケージの作成のみであり、事前に別途ビルドが実行されている必要があります。

### .\release\

リリース用パッケージ作成バッチによって作成されるリリース用パッケージが格納されるディレクトリです。

### .\snapshot\

solomon によって作成されるディレクトリです。

### .\snapshot\master\

solomon がビルド時に .\source\ ディレクトリをミラーしたディレクトリです。
.\testsnap\ ディレクトリへは .\source\ から直接ミラーされるのではなく、こちらのディレクトリから間接的にミラーされます。
こうすることでビルド中にソースコードを変更されてもソースコードとそのビルド結果の組み合わせがブレることがないようにしています。

### .\snapshot\result\

solomon によって作成されたビルドされた結果のファイルが格納されるディレクトリです。

### .\snapshot\misssed.compile\

solomon でのビルドに失敗した時のソースディレクトリがこちらに保存されます。

> 全保存されるので必要に応じて古いヤツは削除してください。

### .\snapshot\passed.compile\

solomon でのビルドに成功した時のソースディレクトリがこちらに保存されます。

> 全保存されるので必要に応じて古いヤツは削除してください。

### .\snapshot\solomon.data\

solomon が前回の実行結果を保存しておく為のディレクトリです。

### .\source\

ソースディレクトリです。
基本的にビルドに必要な全てのファイルはこのディレクトリに含まれ、また同時にビルドに不要なファイルは含みません。

### .\source\resource\

リソース関連のディレクトリです。

### .\source\solomon\

solomon 関連のディレクトリです。

### .\source\solomon\build.cmd

ビルドを実行するバッチファイルです。このコマンドの実行には solomon が必要になります。

### .\source\VERSION.cmd

次回ビルド時に .\source\VERSION.h に展開されるバージョン番号が格納されるバッチファイルです。ビルドが成功する度に自動的にビルド番号がインクリメントされます。メジャーバージョン及びマイナーバージョンについては通常手作業でこのファイルを編集することで設定します。

> 次回ビルド用のバージョン情報となる為、ビルドのプロセスが一通り終わった段階では最新ビルドよりビルド番号が一つ進んだ状態になります。

### .\testsnap\

solomon がビルドを行う時に実際に対象のビルドを実行するディレクトリです。

## ライセンス

Boost Software License - Version 1.0 を採用しています。
詳細は [.\LICENSE_1_0.txt](./LICENSE_1_0.txt) を参照してください。

日本語参考訳: http://hamigaki.sourceforge.jp/doc/html/license.html

## バージョン採番ルール

### バージョン表記のフォーマット

`A.BB.CCC`

### メジャーバージョン番号(`A`)

明らかな非互換の変更が行われた際にインクリメント。
桁数は不定。

### マイナーバージョン番号(`BB`)

機能追加や上位互換と判断できる仕様変更が行われた際にインクリメント。
桁数は2桁固定。

### ビルド番号(`CCC`)

バグフィックスや仕様変更というほどでもない微細な修正が行われた際にインクリ
メント。
桁数は3桁固定。

### 細則

* 各番号は0始まりとする。
* 固定桁に足りない場合は先頭を0埋めする。
* 番号が固定桁で足りなくなった場合は、上位の番号をインクリメントする。
* 上位の番号がインクリメントされた場合、下位の番号は0にリセットする。
