# 2020-pd2-DoodleJump

## Video link
## How to play
- **Player**  
  ![Player](/images/player_Right.png "/images/player_Right.png")  
  Use left and right key to move the player left or right.
- **Bullet**  
  ![Bullet](/images/bullet.png "/images/bullet.png")  
  Player can shoot bullets to blast the monster.  
  Pressing space key to shoot bullet.
- **Platform**
  - *Normal Platform*  
    ![Normal Platform](/images/Normal_Platform.png "/images/Normal_Platform.png ")  
    Player can jump on the normal platform.  
  - *Cracked Platform*  
    ![Cracked Platform](/images/Cracked_Platform1.png "/images/Cracked_Platform1.png")  
    Player can **NOT** jump on the normal platform.
  - *Horizontal Moving Platform*  
    ![Horizontal Moving Platform](/images/Horiziontal_Move_Platform.png "/images/Horiziontal_Move_Platform.png")  
    This platform will automatically move left and right.
  - *One-off Platform*  
    ![One-off Platform](/images/One-off_Platform1.png "/images/One-off_Platform1.png")  
    After player jump on this platform, it will disappear.
- **Hazard**
  - *Monster*  
    ![Monster](/images/monster1.png "/images/monster1.png")  
  - *Hole*  
    ![Hole](/images/hole.png "/images/hole.png")
- **Props**
  - *Spring*  
    ![Spring](/images/spring.png "/images/spring.png")
  - *Propeller Helmet*  
    ![Propeller Helmet](/images/propellerHelmet.png "/images/propellerHelmet.png")  
## Requirements
- Player can jump automatically and can move across the boundary (10%)  
  > *player.cpp :: void Player::move()*
- At least two kinds of platforms (10%)  
  > *platform.h :: class NormalPlatform, CrackedPlatform, HorizontalMovePlatform and OneOffPlatform*
- At least two kinds of game props (10%)  
  > *props.h :: class Spring and PropellerHelmet*
- Hazards  
  - Monster (5%)  
    > *hazard.h :: class Monster*
  - Player can shoot bullets to blast the monster (5%)  
    > *bullet.cpp :: void Bullet::move()*
  - If the monster is dead, remove it from the scene (5%)  
    > *bullet.h :: void move()*
  - At least one another hazard (5%)  
    > *hazard.h :: class Hole*
  - Eliminate/Avoid one hazard someway (5%)  
    > *hazard.cpp :: void Monster::collide()*  
    > When the player step jump on monster, it will be deleted.
- Can play the game infinitely until the player died (10%)  
- Can pause the game (5%)  
  > *doodlejump.cpp :: void DoodleJump::pause()*  
- Show the player’s score during the game (5%)  
  > *doodlejump.cpp :: void DoodleJump::setBackground()*  
- Show GAME OVER and final score when the player died (5%)  
  > *doodlejump.cpp :: void DoodleJump::end()*  
## Polymorphism
Class Platform is the base class of class NormalPlatform and CrackedPlatform. And class HorizontalMovePlatform inherited from class NormalPlatform.  
In *player.cpp* file, *void Player::move()* function uses Platform pointer to call the virtual funtion *collide()*.
## Bonus
- Old scores will be displayed on screen  
- Players are able to change their names  
- Sound effects  
- Enable to turn off the sound effect
