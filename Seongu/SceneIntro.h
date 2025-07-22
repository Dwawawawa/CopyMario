// SceneIntro.h
#pragma once
#include "Scene.h"




class SceneIntro : public Scene
{
public:
    SceneIntro() = default;
    virtual ~SceneIntro() override;

    void Initialize() override;
    void Update(float dTime) override;

private:

	
}; 