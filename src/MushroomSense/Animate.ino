uint16_t currSteps = 0;
uint16_t currDuration = 0;
uint16_t currType = 0;
unsigned long  startTime = 0;

struct Animation {
  uint16_t duration;
  float (*easingFunction)(float, Animation*);
  unsigned long  startTime;
  boolean isLoop;
  boolean isActive;
  int* value;
  int initialValue;
  int targetValue;
};

void updateAnimation(struct Animation* animation){
  unsigned long currTime = millis();
  if(animation->isActive && currTime >= animation->startTime){
    uint16_t elapsedTime = ((int)(currTime - animation->startTime) % animation->duration);
    if(!animation->isLoop){
     elapsedTime = min(((int)(currTime - (animation->startTime))), (int)(animation->duration)); 
    }
    float x = ((float) elapsedTime)/((float) (animation->duration));
    float progress = (animation->easingFunction)(x, animation);
    int valDiff = animation->targetValue - animation->initialValue;
    *(animation->value) = animation->initialValue + round(progress * (float)valDiff);
    if(!animation->isLoop && elapsedTime >= animation->duration){
      animation->isActive = false;
    }
  }
}

void startAnimation(Animation* animation){
  animation->startTime = millis();
  animation->isActive = true;
}

void stopAnimation(Animation* animation){
  animation->isActive = false;
}

float linear(float x, Animation* animation) {
  return x;
}

float easeInOutQuint(float x, Animation* animation) {
  return x < 0.5 ? 16 * x * x * x * x * x : 1 - pow(-2 * x + 2, 5) / 2;
}
 
float easeOutQuad(float x, Animation* animation) {
  return 1 - (1 - x) * (1 - x);
}

float easeInOutExpo(float x, Animation* animation) {
  return x == 0
    ? 0
    : x == 1
    ? 1
    : x < 0.5 ? pow(2, 20 * x - 10) / 2
    : (2 - pow(2, -20 * x + 10)) / 2;
}

float easeInExpo(float x, Animation* animation){
  return x == 0 ? 0 : pow(2, 10 * x - 10);
}

float easeOutExpo(float x, Animation* animation){
  return x == 1 ? 1 : 1 - pow(2, -10 * x);
}

float easeInQuart(float x, Animation* animation) {
  return x * x * x * x;
}

float easeOutElastic(float x, Animation* animation) {
float c4 = (2 * PI) / 3;
return x == 0
  ? 0
  : x == 1
  ? 1
  : pow(2, -10 * x) * sin((x * 10 - 0.75) * c4) + 1;
}

float easeOutBounce(float x, Animation* animation) {
  float n1 = 7.5625;
  float d1 = 2.75;
  
  if (x < 1 / d1) {
      return n1 * x * x;
  } else if (x < 2 / d1) {
      return n1 * (x -= 1.5 / d1) * x + 0.75;
  } else if (x < 2.5 / d1) {
      return n1 * (x -= 2.25 / d1) * x + 0.9375;
  } else {
      return n1 * (x -= 2.625 / d1) * x + 0.984375;
  }
}
