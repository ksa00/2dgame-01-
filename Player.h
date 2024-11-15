#pragma once
#include "Engine/GameObject.h"
#include"PowerUp.h"
#include <string>
#include"Animation.h"

class Player : public GameObject
{
public:
    // �R���X�g���N�^
    // �����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
    Player(GameObject* parent);

    // �f�X�g���N�^
    ~Player();

    // ������
    void Initialize() override;

    // �X�V
    void Update() override;

    // �`��
    void Draw() override;

    // �J��
    void Release() override;

   

    // �p���[�A�b�v��K�p
    void ApplyPowerUp(int powerUpImageHandle);

    // �p���[�A�b�v�����W
    void CollectPowerUp(PowerUp* powerUp);

    

    // �X�R�A���X�V
    void UpdateScore(int points);

    // ���x�����X�V
    void UpdateLevel(int newLevel);

    // �Q�b�^�[
    const std::string& GetName() const;
    int GetAttribute() const;
    int GetHealth() const;
    int GetScore() const;
    int GetLevel() const;

  

private:
    enum State {
        Idle_,
        Run_,
        Jump_,
        Fall_,
        Hit_,
        Attack_,
        Dash_,
        Death_,
        // Add more as needed
    };
    State currentState;
    std::string name;
    int IdleImg;
    int RunImg;
    int JumpImg;
    int FallImg;
    int HitImg;
    int AttackImg;
    int DashImg;
    int DeathImg;
    int Attribute;
    int health;
    int score;
    int level;
    int activePowerUp;
    Animation* animation;
    // �v���C���[����ݒ�
    void SetName(const std::string& playerName);
    void SetHealth(int health);
    void SetAttribute(int handle);
    void SetActivePowerUp(int handle);
    void Run();
    void Jump();
    void fall();
    void Attack();
    void Hit(int amount);
    void Dash();
    void Death();
};
