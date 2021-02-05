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
  - <img src="https://user-images.githubusercontent.com/42861828/106978604-e555d280-679f-11eb-8656-95c5c135e8fd.png" width="20%"> <img src="https://user-images.githubusercontent.com/42861828/106978611-e850c300-679f-11eb-9319-4dbef3aa0bdd.png" width="20%">  


    
- 影の表示および他のShadingモデル
  - Lambert.cpp
  - Mirror.cpp
  - Glass.cpp
  - <img src="https://user-images.githubusercontent.com/42861828/106978492-b8092480-679f-11eb-8bd4-c106e3bd2510.png" width="20%">  


- HDR画像によるImage Based Lightning
  - Lambert.cpp
  - Scene.cpp
  - <img src="https://user-images.githubusercontent.com/42861828/106978484-b63f6100-679f-11eb-8121-54129d6bb849.png" width="20%">  

- PinholeカメラからLensカメラにも対応
  - Camera.cpp
  - <img src="https://user-images.githubusercontent.com/42861828/106978490-b7708e00-679f-11eb-98b9-4d181a1af3b6.png" width="20%">  

  
- Path Tracing対応
  - Lambert.cpp
  - <img src="https://user-images.githubusercontent.com/42861828/106978355-75474c80-679f-11eb-8a32-6eb324d87507.png" width="20%">  

- Motion Blur
  - Scene.cpp
  - <img src="https://user-images.githubusercontent.com/42861828/106978208-226d9500-679f-11eb-8ec0-b9f136819efa.png" width="20%">
