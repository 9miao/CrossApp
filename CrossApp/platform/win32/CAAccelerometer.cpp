

#include "CAAccelerometer.h"
#include "CCEGLView.h"
#include "basics/CAApplication.h"
#include "ccMacros.h"

namespace
{

    double            g_accelX=0.0;
    double            g_accelY=0.0;
    double            g_accelZ=0.0;
    const double    g_accelerationStep=0.2f;
    const double    g_minAcceleration=-1.0f;
    const double    g_maxAcceleration=1.0f;

    template <class T>
    T CLAMP( const T val,const T minVal,const T maxVal )
    {
        CC_ASSERT( minVal<=maxVal );
        T    result=val;
        if ( result<minVal )
            result=minVal;
        else if ( result>maxVal )
            result=maxVal;

        CC_ASSERT( minVal<=result && result<=maxVal );
        return result;
    }

    bool handleKeyDown( WPARAM wParam )
    {
        bool    sendUpdate=false;
        switch( wParam )
        {
        case VK_LEFT:
            sendUpdate=true;
            g_accelX=CLAMP( g_accelX-g_accelerationStep,g_minAcceleration,g_maxAcceleration );
            break;
        case VK_RIGHT:
            sendUpdate=true;
            g_accelX=CLAMP( g_accelX+g_accelerationStep,g_minAcceleration,g_maxAcceleration );
            break;
        case VK_UP:
            sendUpdate=true;
            g_accelY=CLAMP( g_accelY+g_accelerationStep,g_minAcceleration,g_maxAcceleration );
            break;
        case VK_DOWN:
            sendUpdate=true;
            g_accelY=CLAMP( g_accelY-g_accelerationStep,g_minAcceleration,g_maxAcceleration );
            break;
        case VK_OEM_COMMA:
            sendUpdate=true;
            g_accelZ=CLAMP( g_accelZ+g_accelerationStep,g_minAcceleration,g_maxAcceleration );
            break;
        case VK_OEM_PERIOD:
            sendUpdate=true;
            g_accelZ=CLAMP( g_accelZ-g_accelerationStep,g_minAcceleration,g_maxAcceleration );
            break;
        }
        return sendUpdate;
    }

    bool handleKeyUp( WPARAM wParam )
    {
        bool    sendUpdate=false;
        switch( wParam )
        {
        case VK_LEFT:
        case VK_RIGHT:
            sendUpdate=true;
            g_accelX=0.0;
            break;
        case VK_UP:
        case VK_DOWN:
            sendUpdate=true;
            g_accelY=0.0;
            break;
        case VK_OEM_COMMA:
        case VK_OEM_PERIOD:
            sendUpdate=true;
            g_accelZ=0.0;
            break;
        }
        return sendUpdate;
    }

    void myAccelerometerKeyHook( UINT message,WPARAM wParam,LPARAM lParam )
    {
		CrossApp::CAAccelerometer    *pAccelerometer = CrossApp::CAApplication::getApplication()->getAccelerometer();
        bool                        sendUpdate=false;
        switch( message )
        {
        case WM_KEYDOWN:
            sendUpdate=handleKeyDown( wParam );
            break;
        case WM_KEYUP:
            sendUpdate=handleKeyUp( wParam );
            break;
        case WM_CHAR:
            // Deliberately empty - all handled through key up and down events
            break;
        default:
            // Not expected to get here!!
            CC_ASSERT( false );
            break;
        }

        if ( sendUpdate )
        {
            const time_t    theTime=time(NULL);
            const double    timestamp=(double)theTime / 100.0;
            pAccelerometer->update( g_accelX,g_accelY,g_accelZ,timestamp );
        }
    }

    void resetAccelerometer()
    {
        g_accelX=0.0;
        g_accelY=0.0;
        g_accelZ=0.0;
    }

}

NS_CC_BEGIN

CAAccelerometer::CAAccelerometer() :
    m_pAccelDelegate(NULL)
{
    memset(&m_obAccelerationValue, 0, sizeof(m_obAccelerationValue));
}

CAAccelerometer::~CAAccelerometer()
{

}

void CAAccelerometer::setDelegate(CAAccelerometerDelegate* pDelegate) 
{
    m_pAccelDelegate = pDelegate;

    // Enable/disable the accelerometer.
    // Well, there isn't one on Win32 so we don't do anything other than register
    // and deregister ourselves from the Windows Key handler.
    if (pDelegate)
    {
        // Register our handler
        CCEGLView::sharedOpenGLView()->setAccelerometerKeyHook( &myAccelerometerKeyHook );
    }
    else
    {
        // De-register our handler
        CCEGLView::sharedOpenGLView()->setAccelerometerKeyHook( NULL );
        resetAccelerometer();
    }
}

void CAAccelerometer::setAccelerometerInterval(float interval)
{

}

void CAAccelerometer::update(double x, double y, double z, double timestamp)
{
    if (m_pAccelDelegate)
    {
        m_obAccelerationValue.x            = x;
        m_obAccelerationValue.y            = y;
        m_obAccelerationValue.z            = z;
        m_obAccelerationValue.timestamp = timestamp;

        // Delegate
        m_pAccelDelegate->didAccelerate(&m_obAccelerationValue);
    }    
}

NS_CC_END

