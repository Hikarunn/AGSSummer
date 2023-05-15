#pragma once
#include "Transition.h"
class Fade :
    public Transition
{
public:
    Fade(SceneUptr before, SceneUptr after, float fadeTime);
private:
    bool UpdateFadeOut(void);
    bool UpdateFadeIn(void);
    bool IsTransAfter(float delta) final;
    void DrawScene(void) final;
    void DrawFadeOut(void);
    void DrawFadeIn(void);

    void (Fade::* draw_)();
    bool (Fade::* update_)();
    float time_;
    const float fadeTime_;
};

