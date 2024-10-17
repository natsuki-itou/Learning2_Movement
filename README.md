# Learning2_Movement

## 概要
このプロジェクトは 
- Characterを動かす方法
- Animationの設定方法  

をメインに学習するものです。  

## 学習内容
このプロジェクトで学習した内容のメモ

### UnrealEngine5
- IK Retargeter
- Enhanced Input
- CharacterMovementComponent
  - orient rotation to movement
- Controller != nullptrをやる意味
  > コンストラクタでやると初期化が間に合わない場合がある その状態で呼び出すとクラッシュするから
- not buuleanとnot equalは違う 当たり前
### C++
- 

### その他
- 

## 不明点や調査が必要な点
- デリゲート
- Controller->GetControlRotation()とGetActorForwardVector()でとれるベクトルの違い、イメージ
- ベクトル、高校数学

## 参考
- [Character Movement (キャラクターの動き) を設定する | Unreal Engine 5.5 ドキュメンテーション | Epic Developer Community](https://dev.epicgames.com/documentation/ja-jp/unreal-engine/setting-up-character-movement)
- [Unreal Engine でのモーション ワープ | Unreal Engine 5.3 ドキュメンテーション | Epic Developer Community](https://dev.epicgames.com/documentation/ja-jp/unreal-engine/motion-warping-in-unreal-engine?application_version=5.3)
- [猫でも分かる UE4を使ったゲーム開発 超初級編 #6 自作・購入したキャラクタを動かしてみよう！ - YouTube](https://www.youtube.com/watch?v=Xcl0IA3HpfQ)
- [UE5でのアニメーションの流用方法について【Compatible Skeletons, IK Retargeter】 - YouTube](https://www.youtube.com/watch?v=nb8P_VmRKog)  