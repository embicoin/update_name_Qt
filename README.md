# update\_name\_Qt
GUIで使えるupdate\_name

## update\_nameとは
```
@xxxx update_name 新しい名前  
```
というようにツイートするとnameを変更するという仕組みです。  

## update\_name\_Qtの特徴

### GUI
一般的にupdate\_nameにはGUIは付いていませんが、update\_name\_QtではQtを使ったGUIが付いています。  

### クロスプラットフォーム
update\_name\_QtはWindowsだけでなく、MacやLinuxでも動作します。

### 簡単
update_name_Qtはダウンロードした後、すぐに使えます。  
導入はとても簡単です。

## ダウンロード
バイナリは [Releases](https://github.com/owata-programer/update_name_Qt/releases) で公開されています。

* Linux-x64
* Linux-x86
* Mac_OS_X
* Windows-x64
* Windows-x64

がありますが、自分の環境に合ったものをダウンロードしてください。

## インストール

### Linux

#### 1. 展開する
ファイルは圧縮されているので展開する必要があります。  

##### tar.gzの場合
```
$ tar zxvf Linux-x64.tar.gz
```

##### tar.xzの場合
```
$ tar -Jxvf Linux-x86.tar.xz
```

#### 2. 実行する
Linux-x64（またはLinux-x86）のディレクトリの中にupdate_name_Qt.shがあるので、それを実行してください。

### Mac OS X

#### 1. 展開する
ファイルは圧縮されているので展開する必要があります。  
OS標準のアーカイブマネージャで大丈夫です。

#### 2. 実行する
update\_name\_Qtを実行してください。  
「開発元が未確認のため開けません」というエラーが出た場合は、Controlキーを押しながらクリックして、「開く」を選択してください。

### Windows

#### 1. 「Visual Studio 2013 の Visual C++ 再頒布可能パッケージ」をインストールする
__注意：既にインストールしている場合はこの手順を飛ばしても構いません。__

update_name_QtのWindows版はVisual C++でコンパイルされているので、Visual C++のランタイムが必要です。
http://www.microsoft.com/ja-jp/download/details.aspx?id=40784  
Windows-x64の方を使う場合は「vcredist_x64.exe」、Windows-x86の方を使う場合は「vcredist_x86.exe」をインストールしてください。

#### 2. 展開する
ファイルは圧縮されているので展開する必要があります。
右クリックして「すべて展開する」をクリックして展開してください。

#### 3. 実行する
udpate\_name\_Qt.exeを実行してください。

> Windows によって PC が保護されました  
> Windows SmartScreen は認識されないアプリの起動を停止しました。このアプリを実行すると、PC に問題が起こる可能性があります。

このようなエラーが出て実行できない場合は、「詳細情報」をクリックして、「実行」をクリックしてください。

## 使い方

### 1. 認証
初回起動時には認証ダイアログが表示されます。  
「開く」ボタンをクリックしてTwitterの認証ページをウェブブラウザで開き、Twitterアカウントで認証してください。  
ピンコードが表示されたら、認証ダイアログにピンコードを入力して、「OK」をクリックしてください。

### 2. update\_nameを実行する

「update\_nameスタート」をクリックすると、update\_nameがスタートされます。  

### 3. 設定
ツール→設定から各種設定を行うことができます。

#### 動作

##### 常駐
「メインウィンドウを閉じてもシステムトレイに常駐する」にチェックを入れると、メインウィンドウを閉じてもシステムトレイトレイに常駐して動作を続けます。

##### update_name
「update\_name\_Qt」を起動して時にupdate\_nameを自動的にスタート」にチェックを入れると、「update\_nameスタート」をクリックしなくても自動的にupdate\_nameがスタートします。

#### コマンド
update\_name\_Qtはupdate\_name以外にもコマンドがあります。

##### update\_name
プロフィールのnameを変更します。

使い方

```
@xxx update_name 新しい名前
```

##### update\_url
プロフィールのURLを変更します。

使い方

```
@xxx update_url http://example.com/
```

##### update\_location
プロフィールの場所を変更します。

使い方

```
@xxx update_location お布団
```

##### update_description
プロフィールの自己紹介欄を変更します。

使い方

```
@xxx update_description 24歳、学生です。
```

##### update_image
プロフィールのアバター画像を変更します。

使い方

```
@xxx update_image pic.twitter.com/XXXXXX
```

#### update\_background\_image
ユーザーの背景画像を変更します。

使い方

```
@xxx update_background_image pic.twitter.com/XXXXXX
```

#### メッセージ
起動時やupdate_nameを受けた際のツイートを変更できます。  

#### 認証
Twitterでの認証をやり直したりすることができます。

#### デバッグ
デバッグ用

## バグ報告
バグを発見した場合は

* GitHubのIssuesに登録する
* [Twitter](https://twitter.com/owata_programer)で報告する

などしてください。