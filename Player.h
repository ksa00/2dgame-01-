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
    Player(GameObject* parent, const std::string& name, int attributeImageHandle);

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

    // �U��
    void Attack();

    // �_���[�W���󂯂�
    void TakeDamage(int amount);

    // �p���[�A�b�v��K�p
    void ApplyPowerUp(int powerUpImageHandle);

    // �p���[�A�b�v�����W
    void CollectPowerUp(PowerUp* powerUp);

    // �v���C���[����ݒ�
    void SetName(const std::string& playerName);

    // �X�R�A���X�V
    void UpdateScore(int points);

    // ���x�����X�V
    void UpdateLevel(int newLevel);

    // �Q�b�^�[
    const std::string& GetName() const;
    int GetAttributeImageHandle() const;
    int GetHealth() const;
    int GetScore() const;
    int GetLevel() const;

private:
    int Shadow_img;
    std::string name;
    int attributeImageHandle;
    int health;
    int score;
    int level;
    int activePowerUpImageHandle;
    AnimationManager animationManager;
    Animation* currentAnimation;
};
