# miro_whim
このコードはRealistic Image Synthesis 2020の授業で公開されたレンダリングシミュレータMiroを基に開発したものです。
元のコードに対して以下の実装(コードの変更・追加)を行いました

- Bounding Volume Hierarchy (for Ray Tracing)
  - BVH.cpp
  - BoundingVolume.h
 
- BVHを用いたRayTracing
  - Accel.cpp
  - Triangle.cpp
    - クラメルの法則を用いた衝突判定&法線ベクトル
    
- 影の表示および他のShadingモデル
  - Lambert.cpp
  - Mirror.cpp
  - Glass.cpp

- HDR画像によるImage Based Lightning
  - Lambert.cpp
  - Scene.cpp
  
- PinholeカメラからLensカメラにも対応
  - Camera.cpp
  
- Path Tracing対応
  - Lambert.cpp
  
- Motion Blur
  - Scene.cpp
