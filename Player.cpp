#include "Player.h"
#include "Enemy.h"
#include "CombatSystem.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Debug.h"
#include "Attributes.h"
#include <cassert>

Player::Player(GameObject* parent)
    : GameObject(parent, "Player"),
    currentState(Death_),
    name(""),
    health(10),
    score(0),
    level(1),
    activePowerUp(-1),
    attackPower(10),
    baseAnimation(nullptr),
    deathAnimationComplete(false), // Initialize flag to false
    isGrounded(true),
    groundLevel(500.0f),
    facingRight(true),
    speed(5.0f),
    jumpVelocity(0.0f),
    jumpForce(20.0f),
    gravity(1.5f),
    attackTimer(0.0f),
    attackCooldown(0.3f) // Cooldown between attacks (in seconds)
{
    enemy = nullptr;
    IdleImg = -1;
    RunImg = -1;
    JumpImg = -1;
    FallImg = -1;
    HitImg = -1;
    AttackImg = -1;
    DashImg = -1;
    DeathImg = -1;
    totalDashDistance = 200.0f; // Example distance for a dash
    // Example frame count for the dash animation
    distancePerFrame = totalDashDistance / 10; // Calculate distance per frame based on total dash distance and frame count
}

Player::~Player() {
    delete baseAnimation;
    for (auto anim : overlayAnimations) {
        delete anim;
    }
}

void Player::Initialize() {
    IdleImg = Image::Load("Player/Idle.png");
    assert(IdleImg >= 0);
    RunImg = Image::Load("Player/Run.png");
    assert(RunImg >= 0);
    JumpImg = Image::Load("Player/Jump.png");
    assert(JumpImg >= 0);
    FallImg = Image::Load("Player/Fall.png");
    assert(FallImg >= 0);
    HitImg = Image::Load("Player/Hit.png");
    assert(HitImg >= 0);
    AttackImg = Image::Load("Player/Attack.png");
    assert(AttackImg >= 0);
    DashImg = Image::Load("Player/Dash.png");
    assert(DashImg >= 0);
    DeathImg = Image::Load("Player/Death_Test.png");
    assert(DeathImg >= 0);

    transform_.position_ = { 50.0f, groundLevel, 0.0f };
    baseAnimation = new Animation(12, 0.055f, IdleImg);
    enemy = dynamic_cast<Enemy*>(FindObject("Enemy")); // Initialize enemy pointer
}

void Player::Update() {
    static int updateCount = 0;
    updateCount++;

    if (!IsDead()) {
        HandleInput();
        ApplyGravity();
        CheckLanding();
    }
    else if (currentState == Death_ && !deathAnimationComplete) {
        // Check if the death animation has completed
        deathAnimationComplete = baseAnimation->IsAnimationComplete();
        if (deathAnimationComplete) {
            KillMe();
        }
    }

    attackTimer -= 1.0f / 60.0f; // Assuming 60 FPS
    Debug::Log("Player Update Call #" + std::to_string(updateCount)); // Log each update call
    baseAnimation->Update();
    for (auto anim : overlayAnimations) {
        anim->Update();
    }
}

void Player::Draw() {
    baseAnimation->Draw(transform_, facingRight);
    for (auto anim : overlayAnimations) {
        anim->Draw(transform_, facingRight);
    }
}

void Player::Release() {
    // Release resources
}

void Player::HandleInput() {
    if (IsDead()) {
        return;
    }

    bool isRunning = Input::IsKey(DIK_A) || Input::IsKey(DIK_D);
    bool isJumping = Input::IsKeyDown(DIK_W);
    bool isDashing = Input::IsKeyDown(DIK_LSHIFT);
    bool isAttacking = Input::IsKeyDown(DIK_SPACE);

    if (isRunning && isJumping) {
        Jump();
        Run();
    }
    else if (isRunning && isDashing) {
        Dash();
        Run();
    }
    else if (isRunning && isAttacking && attackTimer <= 0.0f) {
        PerformAttack();
        attackTimer = attackCooldown;
        Run();
    }
    else if (isRunning) {
        Run();
    }
    else if (isJumping) {
        Jump();
    }
    else if (isDashing) {
        Dash();
    }
    else if (isAttacking && attackTimer <= 0.0f) {
        PerformAttack();
        attackTimer = attackCooldown;
    }
    else if (isGrounded) {
        SetAnimationState(Idle_);
    }

    if (Input::IsKey(DIK_A)) {
        transform_.position_.x -= speed;
        facingRight = false;
    }
    if (Input::IsKey(DIK_D)) {
        transform_.position_.x += speed;
        facingRight = true;
    }
}

void Player::ApplyGravity() {
    if (!isGrounded) {
        jumpVelocity += gravity;
        transform_.position_.y += jumpVelocity;
    }
}

void Player::CheckLanding() {
    if (!isGrounded && transform_.position_.y >= groundLevel) {
        isGrounded = true;
        transform_.position_.y = groundLevel;
        jumpVelocity = 0.0f;
        SetAnimationState(Fall_);
    }
    else if (isGrounded && transform_.position_.y < groundLevel) {
        isGrounded = false;
    }
}

void Player::SetAnimationState(State newState) {
    if (IsDead()) {
        return;
    }

    if (currentState != newState) {
        currentState = newState;
        Debug::Log("Changing state to " + std::to_string(newState)); // Debug log

        switch (newState) {
        case Idle_:
            baseAnimation->SetAnimation(12, 0.055f, IdleImg);
            break;
        case Run_:
            baseAnimation->SetAnimation(8, 0.075f, RunImg);
            break;
        case Jump_:
            baseAnimation->SetAnimation(4, 0.15f, JumpImg, 0, false);
            isGrounded = false;
            break;
        case Fall_:
            baseAnimation->SetAnimation(4, 0.1f, FallImg);
            break;
        case Attack_:
            if (!activeAttributes.empty()) {
                activeAttributes.front()->SetAttackAnimation(baseAnimation);
            }
            break;
        case Dash_:
            baseAnimation->SetAnimation(10, 0.25f, DashImg, 0, 5, false);
            break;
        case Hit_:
            baseAnimation->SetAnimation(4, 0.1f, HitImg, 0, false);
            break;
        case Death_:
            baseAnimation->SetAnimation(10, 0.055f, DeathImg, false); // Play once, no loop
            break;
        }
    }
}

void Player::Run() {
    SetAnimationState(Run_);
}

void Player::Jump() {
    if (isGrounded) {
        jumpVelocity = -jumpForce;
        isGrounded = false;
        SetAnimationState(Jump_);
    }
}

void Player::Fall() {
    SetAnimationState(Fall_);
}

void Player::Dash() {
   
    float dashMovement = distancePerFrame;
    transform_.position_.x += dashMovement * (facingRight ? 1 : -1);
    SetAnimationState(Dash_);
}

void Player::TakeDamage(int amount) {
    if (IsDead()) {
        return;
    }

    health -= amount;
    if (health > 0) {
        SetAnimationState(Hit_);
    }
    else {
        SetAnimationState(Death_);
    }
    // The KillMe call is now handled in the Update method after animation completion
}

void Player::PerformAttack() {
    enemy = dynamic_cast<Enemy*>(FindObject("Enemy")); // Ensure enemy is retrieved
    if (enemy) {
        CombatSystem::PerformAttack(this, enemy); // Use CombatSystem to handle the attack
    }
}

const std::string& Player::GetName() const {
    return name;
}

Attribute* Player::GetAttribute() const {
    if (!acquiredAttributes.empty()) {
        return acquiredAttributes.front(); // Just returning the first for demonstration
    }
    return nullptr;
}

int Player::GetHealth() const {
    return health;
}

int Player::GetScore() const {
    return score;
}

int Player::GetLevel() const {
    return level;
}

int Player::GetAttackPower() const {
    return attackPower;
}

void Player::AddAttribute(Attribute* attribute) {
    acquiredAttributes.push_back(attribute);
}

void Player::SetActiveAttributes(const std::vector<Attribute*>& attributes) {
    activeAttributes = attributes;
}

const std::vector<Attribute*>& Player::GetActiveAttributes() const {
    return activeAttributes;
}

void Player::SetName(const std::string& playerName) {
    name = playerName;
}

void Player::SetHealth(int newHealth) {
    health = newHealth;
}

void Player::ApplyPowerUp(int powerUpImageHandle) {
    activePowerUp = powerUpImageHandle;
    // Logic to apply power-up effects
}

void Player::CollectPowerUp(PowerUp* powerUp) {
    ApplyPowerUp(powerUp->GetImageHandle());
}

void Player::UpdateScore(int points) {
    score += points;
}

void Player::UpdateLevel(int newLevel) {
    level = newLevel;
}
