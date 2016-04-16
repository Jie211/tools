tools for solver
====
## Add Python version 
* MatrixChecker 行列の情報を調べる
	- 行の最大要素数と最小要素数
	- 平均行要素数
* MatrixTransfor-fix-qsort MM(MatrixMarket)の行列をCSRに変換クイックソートを利用
	- 右辺ベクトルは最終解が1になれるように計算
* MatrixTransfor-fix-qsort-bvec 上記と同じ
	- 右辺ベクトルはMMのbvecを利用する
* MatrixTransfor-insym 非対称行列用
* MatrixTransfor-insym 非対称行列用、右辺ベクトルはMMのbvecを利用する
* diagcheck　行列の対称性を確認
* epslioncheck　計算機イプシロンを確認

時間があれば更新する。
