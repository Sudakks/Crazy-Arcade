#include "summondummy.h"

#include <gameobject.h>
#include <transform.h>
#include <QTimer>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include "gamescene.h"
#include "health.h"

SummonDummy::SummonDummy() {}

void SummonDummy::onAttach() {
  auto transform = this->gameObject->getComponent<Transform>();
  Q_ASSERT(transform != nullptr);
  // Handle mouse event of text by this->transform
  transform->setFiltersChildEvents(true);
}

void SummonDummy::onClick(QGraphicsSceneMouseEvent *ev) {
    if(stop)
    {
        stop = false;
        this->gameObject->get_scene()->get_updateTimer()->start();
    }
    else
    {
        stop = true;
        this->gameObject->get_scene()->get_updateTimer()->stop();
    }
}
