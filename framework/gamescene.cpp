#include "gamescene.h"

#include <QKeyEvent>
#include <QTimer>
#include <utility>

#include "gameobject.h"
#include "transform.h"

GameScene::GameScene() {
  updateTimer = new QTimer(this);
  updateTimer->setInterval(1000 / 60);//以毫秒为单位
  connect(updateTimer, &QTimer::timeout, this, &GameScene::onUpdate);
  updateTimer->start();
}

GameScene::~GameScene() {
  updateTimer->stop();
  for (auto gameObject : gameObjects) {
    gameObject->onDetach();
    gameObject->setParentGameScene(nullptr);
  }
}

void GameScene::attachGameObject(GameObject *gameObject) {
  gameObjectsToAttach.emplace_back(gameObject);
  //Adds a new element to the end for the container
}
void GameScene::detachGameObject(GameObject *gameObject) {
  gameObjectsToDetach.emplace_back(gameObject);
}
GameObject *GameScene::getGameObject(const char *name) {
  for (auto gameObject : gameObjects) {
    if (gameObject->objectName() == name) return gameObject;
  }
  return nullptr;
}

void GameScene::onUpdate() {
  // Do firstUpdate() for game objects
  for (auto gameObject : gameObjectsAttachedOnLastUpdate) {
    gameObject->onFirstUpdate();
  }

  // Do update() for game objects
  for (auto gameObject : gameObjects) {
    gameObject->onUpdate(updateTimer->interval() / 1000.0);
  }

  // Do detach() for game objects
  for (auto gameObject : gameObjectsToDetach) {
    gameObject->onDetach();
    gameObject->setParentGameScene(nullptr);
    this->gameObjects.removeAll(gameObject);
    //移走所有是gameObject的物品，返回数量
    auto tf = gameObject->getComponent<Transform>();
    if (tf != nullptr) this->removeItem(tf);
  }
  gameObjectsToDetach.clear();

  // Do attach() for game objects
  for (auto gameObject : gameObjectsToAttach) {
    gameObject->setParentGameScene(this);
    this->gameObjects.append(gameObject);
    auto tf = gameObject->getComponent<Transform>();
    if (tf != nullptr) this->addItem(tf);
    gameObject->onAttach();
  }
  std::swap(gameObjectsToAttach, gameObjectsAttachedOnLastUpdate);
  gameObjectsToAttach.clear();

  // Keyboard input
  keyDownTable.clear();
  keyUpTable.clear();
}

void GameScene::keyPressEvent(QKeyEvent *ev) {
  if (ev->isAutoRepeat()) return;
  keyTable[ev->key()] = true;
  keyDownTable[ev->key()] = true;
  QGraphicsScene::keyPressEvent(ev);
}
void GameScene::keyReleaseEvent(QKeyEvent *ev) {
  if (ev->isAutoRepeat()) return;
  keyTable[ev->key()] = false;
  //Returns the code of the key that was pressed or released
  keyUpTable[ev->key()] = true;
  QGraphicsScene::keyReleaseEvent(ev);
}
void GameScene::focusOutEvent(QFocusEvent *ev) {
  keyTable.clear();
  keyDownTable.clear();
  QGraphicsScene::focusOutEvent(ev);
}
