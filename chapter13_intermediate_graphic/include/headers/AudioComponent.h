#include "Component.h"
#include <unordered_map>
#include "SoundEvent.h"
#include <vector>
class AudioComponent : public Component {

public:
    AudioComponent(class Actor* owner, int updateOrder = 10);

    ~AudioComponent();
    void Update(float deltaTime) override;
    void OnUpdateWorldTransform() override;
    SoundEvent PlayEvent(const std::string& name);
    void StopAllEvents();
private:
    std::vector<SoundEvent> mEvents2D;
    std::vector<SoundEvent> mEvents3D;
};