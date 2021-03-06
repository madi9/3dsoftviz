#include "Leap/LeapActions.h"
#include "Core/Core.h"
#include "Viewer/CameraManipulator.h"
#include "Viewer/MouseControl.h"

Leap::LeapActions::LeapActions()
{
	mouse = new Vwr::MouseControl();
	cmrManipulator = AppCore::Core::getInstance( NULL )->getCoreWindow()->getCameraManipulator();
	isCameraMoving = true;
	qDebug() << "LeapActions() Constructor";
}

void Leap::LeapActions::moveCamera( Leap::Gesture gesture )
{
	// swipe gesture
	SwipeGesture swipe = gesture;
	Vector direction = swipe.direction();
	float gestureDuration = static_cast<float>( gesture.duration() )/1000.0f;

	// compare direction[0] and direction[1] to find out what kind of movement are we working with
	if ( abs( direction[0] ) > abs( direction[1] ) ) { // horizontal movement
		if ( direction[0] > 0 ) {
			cmrManipulator->enableCameraMovement( Vwr::CameraManipulator::Movement::RIGHT );
			Sleep( ( DWORD )gestureDuration );
			cmrManipulator->disableCameraMovement();
			qDebug() << "SwipeGesture - right";
		}
		else if ( direction[0] < 0 ) {
			cmrManipulator->enableCameraMovement( Vwr::CameraManipulator::Movement::LEFT );
			Sleep( ( DWORD )gestureDuration );
			cmrManipulator->disableCameraMovement();
			qDebug() << "SwipeGesture - left";
		}
	}
	else if ( abs( direction[0] ) < abs( direction[1] ) ) { // vertical movement
		if ( direction[1] > 0 ) {
			cmrManipulator->enableCameraMovement( Vwr::CameraManipulator::DOWN );
			Sleep( ( DWORD )gestureDuration );
			cmrManipulator->disableCameraMovement();
			qDebug() << "SwipeGesture - up";
		}
		else if ( direction[1] < 0 ) {
			cmrManipulator->enableCameraMovement( Vwr::CameraManipulator::UP );
			Sleep( ( DWORD )gestureDuration );
			cmrManipulator->disableCameraMovement();
			qDebug() << "SwipeGesture - down";
		}
	}
	else {
		if ( direction[2] > 0 ) {
			cmrManipulator->enableCameraMovement( Vwr::CameraManipulator::BACKWARD );
			Sleep( ( DWORD )gestureDuration );
			cmrManipulator->disableCameraMovement();
			qDebug() << "SwipeGesture - up";
		}
		else if ( direction[2] < 0 ) {
			cmrManipulator->enableCameraMovement( Vwr::CameraManipulator::FORWARD );
			Sleep( ( DWORD )gestureDuration );
			cmrManipulator->disableCameraMovement();
			qDebug() << "SwipeGesture - down";

		}
	}
}

void Leap::LeapActions::zoomGraph( Leap::Gesture gesture )
{
	CircleGesture circle = gesture;

	if ( circle.pointable().direction().angleTo( circle.normal() ) <= PI/2 ) {
		qDebug() << "[onFrame()::CircleGesture - clockwise]";
		switch ( gesture.state() ) {
			case Leap::Gesture::STATE_START:
				cmrManipulator->enableCameraMovement( Vwr::CameraManipulator::FORWARD );
				break;
			case Leap::Gesture::STATE_STOP:
				cmrManipulator->disableCameraMovement();
				break;
		}
	}
	else {
		qDebug() << "[onFrame()::CircleGesture - counterclockwise]";
		switch ( gesture.state() ) {
			case Leap::Gesture::STATE_START:
				cmrManipulator->enableCameraMovement( Vwr::CameraManipulator::BACKWARD );
				break;
			case Leap::Gesture::STATE_STOP:
				cmrManipulator->disableCameraMovement();
				break;
		}
	}
}

void Leap::LeapActions::onKeyTap( Leap::Gesture gesture )
{
	if ( isCameraMoving ) {
		isCameraMoving = false;
	}
	else {
		isCameraMoving = true;
	}
	qDebug() << "KeyTapGesture";
}
/*
void LeapActions::onScreenTap( Leap::Gesture gesture )
{
    // screen tap gesture
    qDebug() << "ScreenTapGesture";
}
*/
void Leap::LeapActions::rotateGraph( Leap::Gesture gesture )
{

	// swipe gesture
	SwipeGesture swipe = gesture;
	Vector direction = swipe.direction();

	if ( abs( direction[0] ) > abs( direction[1] ) ) { // horizontal movement
		if ( direction[0] > 0 ) {
			cmrManipulator->rotateCamera( 0, 0, 1, 0, 0.05f );
		}
		else if ( direction[0] < 0 ) {
			cmrManipulator->rotateCamera( 0, 0, 1, 0, -0.05f );
		}
	}
	else { // vertical movement
		if ( direction[1] > 0 ) {
			cmrManipulator->rotateCamera( 0, 0 ,1, 0.05f, 0 );
		}
		else if ( direction[1] < 0 ) {
			cmrManipulator->rotateCamera( 0, 0, 1, -0.05f, 0 );
		}
	}
}

void Leap::LeapActions::changeViewAngle( Leap::DirectionDetector::Direction direction )
{

	switch ( direction ) {
		case Leap::DirectionDetector::Direction::LEFT :       
			cmrManipulator->rotateCamera( 0, 0, 1, 0, -0.01f );
            break;
		case Leap::DirectionDetector::Direction::RIGHT :
			cmrManipulator->rotateCamera( 0, 0, 1, 0, 0.01f );
			break;
		case Leap::DirectionDetector::Direction::UP :
			cmrManipulator->rotateCamera( 0, 0, 1, -0.01f, 0 );
			break;
		case Leap::DirectionDetector::Direction::DOWN :
			cmrManipulator->rotateCamera( 0, 0, 1, 0.01f, 0 );
			break;
		case Leap::DirectionDetector::Direction::STEADY :
			// stuff
			break;
	}

}

void Leap::LeapActions::startMovingForward()
{
	cmrManipulator->enableCameraMovement( Vwr::CameraManipulator::FORWARD );
}

void Leap::LeapActions::stopMovingForward()
{
	cmrManipulator->disableCameraMovement();
}

//jurik
void Leap::LeapActions::graphRotateSwipe(int direction){
    //direction -1 = LEFT, 1 = RIGHT
    switch(direction){
    case -1:
    {
        AppCore::Core::getInstance( NULL )->getCoreGraph()->rotateGraph(direction);
    }
    case 1:
    {
        AppCore::Core::getInstance( NULL )->getCoreGraph()->rotateGraph(direction);
    }
}
}

/*void Leap::LeapActions::scaleGraph(bool scaleUp){
    Layout::LayoutThread* layout = AppCore::Core::getInstance()->getLayoutThread();
    Vwr::CoreGraph* coreGraph = AppCore::Core::getInstance( NULL )->getCoreGraph();
    float distance = layout->getAlg()->getMaxDistance();

    if(scaleUp){
        layout->pause();
        coreGraph->setNodesFreezed( true );
        layout->getAlg()->setMaxDistance(distance * 1.05);
        coreGraph->scaleGraphToBase();
        coreGraph->setNodesFreezed( false );
        layout->play();
    }
    else{
        layout->pause();
        coreGraph->setNodesFreezed( true );
        layout->getAlg()->setMaxDistance(distance * 0.95 );
        coreGraph->scaleGraphToBase();
        coreGraph->setNodesFreezed( false );
        layout->play();
    }
}*/

void Leap::LeapActions::rotateAruco(Leap::DirectionDetector::Direction direction )
{
    switch ( direction ) {
    case Leap::DirectionDetector::Direction::LEFT :
        AppCore::Core::getInstance( NULL )->getCoreGraph()->rotateGraph(1);
        break;
    case Leap::DirectionDetector::Direction::RIGHT :
        AppCore::Core::getInstance( NULL )->getCoreGraph()->rotateGraph(-1);
        break;
    /*case Leap::DirectionDetector::Direction::DOWN :
        AppCore::Core::getInstance( NULL )->getCoreGraph()->rotateGraph(2);
        break;
    case Leap::DirectionDetector::Direction::UP :
        AppCore::Core::getInstance( NULL )->getCoreGraph()->rotateGraph(-2);
        break;*/
    case Leap::DirectionDetector::Direction::STEADY :
        break;
    }
}

void Leap::LeapActions::scaleEdges(Leap::DirectionDetector::Direction direction )
{
    Layout::LayoutThread* layout = AppCore::Core::getInstance()->getLayoutThread();
    Vwr::CoreGraph* coreGraph = AppCore::Core::getInstance( NULL )->getCoreGraph();
    float distance = layout->getAlg()->getMaxDistance();

    switch ( direction ) {
    case Leap::DirectionDetector::Direction::LEFT :
        layout->pause();
        coreGraph->setNodesFreezed( true );
        layout->getAlg()->setMaxDistance(distance * 1.02);
        coreGraph->scaleGraphToBase();
        coreGraph->setNodesFreezed( false );
        layout->play();
        break;
    case Leap::DirectionDetector::Direction::RIGHT :
        layout->pause();
        coreGraph->setNodesFreezed( true );
        layout->getAlg()->setMaxDistance(distance * 0.98 );
        coreGraph->scaleGraphToBase();
        coreGraph->setNodesFreezed( false );
        layout->play();
        break;
    case Leap::DirectionDetector::Direction::STEADY :
        break;
    }
}

void Leap::LeapActions::scaleNodes(bool scaleUp)
{
    Vwr::CoreGraph* coreGraph = AppCore::Core::getInstance( NULL )->getCoreGraph();

    if(scaleUp)
        coreGraph->scaleNodes(true);
    else
        coreGraph->scaleNodes(false);
}

//*****
