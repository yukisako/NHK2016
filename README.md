#NHKロボコン2016 ハイブリッドロボット

###概要

STM32を動かすための，C++のコードと，arduinoを制御する為のファイルからなる．

##main.cppについて
####i2c通信については以下を参照

i2c通信について: http://www.picfun.com/c15.html

i2c通信関係のリファレンス: https://developer.mbed.org/users/okini3939/notebook/I2C_jp/

途中で行っているビットシフトの処理について: http://www.geocities.jp/zattouka/GarageHouse/micon/I2C/I2C_1.htm

i2c通信で，dataを取得することができる．


##arduino_i2c.inoについて

###概要

コントローラで操作を行うためのファイル．

githubに上がっている，https://github.com/felis/USB_Host_Shield_2.0 をzipで落としてきて，Arduinoから「ライブラリをインクルード」→「zip形式のライブラリをインストール」を選ぶ．

PS3USB.hとspi4teensy3.hが使えるようになる．

コントローラのデータをArduino側で処理し，i2c通信でSTM32に送ることができる．


###ボタン対応表

各データの値が以下のようになっている．

TX_data[0]=○　

TX_data[1]=×

TX_data[2]=△　

TX_data[3]=□

TX_data[4]=→　

TX_data[5]=↓

TX_data[6]=↑　

TX_data[7]=←

TX_data[8]=R1　

TX_data[9]=L1

TX_data[10]=R2　

TX_data[11]=L2

TX_data[12]=RX　

TX_data[13]=RY

TX_data[14]=LX　

TX_data[15]=LY


##issue

* if文のところはわざわざ分岐使わなくても直で代入できる

* defineして可読性高める

* いろんなところを関数化

* STM32でサーボを動かす処理は未実装
