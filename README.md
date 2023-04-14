# GraphicsTutorial
This is a personal project reference to https://www.youtube.com/@makinggameswithben/featured</br>

## 2D Game Engine</br>
### Classes
- IOManager : Read File
- GLSLProgram : Compile and Link Shaders
  - Use IOManager to read vertexShader and fragmentShader
  - addAttribute(), getUniformLocation()
- TextureCache
- ResourceManager
- ImageLoader
- InputManager

- Sprite
- SpriteBatch : Draw groups of sprites onto the screen
- SpriteFont

- ScreenList

- Camera2D : Play as eyes of player
  - Update with Homogeneous Camera Matrix
  - Convert Screen Coordinate to World Coordinate
  - Optimization with Camera Culling (Ignore out of view)

- Window : Create Program Window
  - Open an SDL Window : SDL_CreateWindow
  - Contain screenWidth, screenHeight
- Timing : Set Max FPS and Calculate FPS

- ParticleBatch2D
- ParticleEngine2D

### Interfaces
- IMainGame
- IGameScreen

#
## Zombie Game</br>
- Map Setting with Level.txt by using File I/O</br>
- Class Inheritance : Agent <- Human <- Player</br>
- Moving(SDL_A, S ,D, W), Change Weapon(SDL_1, 2, 3), Shoot toward Mouse Coordinate</br>
- Collision Detection with AABB and Circle Radius</br>
- Display Text UI by using SDL_ttf</br>
- Particle Effect with Random Direction Vector and Life Time

https://user-images.githubusercontent.com/76864202/231960375-ca9ae5c4-047d-4ced-b9cc-98f05126f8c4.mp4

#
## Ball Graphics</br>
- Use Simple Physics : Billiards Collision, Position-Velocity-Acceleration Relation</br>
- Multiple Renderer : Render Mode with Transition, Momentum, Velocity</br>
- Optimization with Spatial Partitioning</br>

https://user-images.githubusercontent.com/76864202/231959477-14b39608-62eb-4864-a790-1f4133ca3a98.mp4

#
## Ninja Platformer</br>
- Box2D Physics 101</br>
- Box Collider && Capsule Collider</br>
- 2D Animation w/ sprite and uvRect</br>
- Basic 2D Lighting</br>

//play video comes here</br>

#
TODO : </br>
1. Mouse Coordinate Error : x coord is tracking wrong!!
