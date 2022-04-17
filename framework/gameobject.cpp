#include "gameobject.h"

#include "component.h"
#include "gamescene.h"
#include "transform.h"

GameObject::GameObject() : QObject() {}
GameObject::~GameObject() {
  if (this->gameScene != nullptr) {
    detachGameObject(this);
  }
}

void GameObject::addComponent(Component *component) {
  component->setParentGameObject(this);
  //就是将这个component的特性加到这个gameobject上
  components.append(component);
}
void GameObject::removeComponent(Component *component) {
  component->setParentGameObject(nullptr);
  components.removeAll(component);
}

void GameObject::onAttach() {
  for (auto component : components) {
    component->onAttach();
  }
}
void GameObject::onFirstUpdate() {
  for (auto component : components) {
    component->onFirstUpdate();
  }
}
void GameObject::onUpdate(float deltaTime) {
  for (auto component : components) {
    component->onUpdate(deltaTime);
  }
}
void GameObject::onDetach() {
  for (auto component : components) {
    component->onDetach();
  }
}

void GameObject::attachGameObject(GameObject *gameObject) {
  this->gameScene->attachGameObject(gameObject);
}
void GameObject::detachGameObject(GameObject *gameObject) {
  this->gameScene->detachGameObject(gameObject);
}

void GameObject::destory(GameObject *gameObject) {
  detachGameObject(gameObject);
  gameObject->deleteLater();
}

bool GameObject::getKey(Qt::Key key) { return this->gameScene->getKey(key); }

bool GameObject::getKeyDown(Qt::Key key) {
  return this->gameScene->getKeyDown(key);
}

bool GameObject::getKeyUp(Qt::Key key) {
  return this->gameScene->getKeyUp(key);
}

void GameObject::onClick(QGraphicsSceneMouseEvent *ev) {
  for (auto component : components) {
    component->onClick(ev);
    //让它的组件执行mouse的要求
  }
}

void GameObject::setParentGameScene(GameScene *gameScene) {
  this->setParent(gameScene);
  this->gameScene = gameScene;
}
