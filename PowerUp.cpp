#include "PowerUp.h"

PowerUp::PowerUp(GameObject* parent)
    : GameObject(parent),PowerImg(-1) {}

void PowerUp::Initialize() {
    // �������R�[�h
}

void PowerUp::Update() {
    // �X�V�R�[�h
}

void PowerUp::Draw() {
    // �`��R�[�h
}

void PowerUp::Release() {
    // �J���R�[�h
}

int PowerUp::GetImageHandle() const
{
    return PowerImg;
}

