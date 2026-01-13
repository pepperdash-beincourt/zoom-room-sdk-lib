/*!
 * \file ICalibrationHelper.h
 * \since Added in ZRC SDK 6.1.0.
 * \brief Calibration interfaces.
 * 
 * 
 *  **How to call the APIs to calibrate Intelligent Director(ID) and configure Camera Boundary(CB):** 
 *      1. **How to call the APIs to calibrate Intelligent Director:**
 * 
 *          1. You need to select 2 or 3 cameras for the Intelligent Director mode through calling the \link SelectIntelligentDirectorCamera \endlink API;
 *          2. Call the \link StartIntelligentDirectorCalibration \endlink API to start the Intelligent Director Calibration;
 *          3. When the current calibration action changes, you will receive the \link OnIntelligentDirectorCalibrationActionChanged \endlink sink, includes the available actions, call the APIs to continue;
 *          4. After calling the \link IDCalibrationActionAcceptCalibrationResult \endlink API, calibration is done. You continue to configure id Camera Boundary if supported;
 *          5. After calling the \link IDCalibrationActionAcceptBoundaryResult \endlink API, ID Camera Boundary is done. Configure Intelligent Director successfully;
 *          6. Call the \link StopIntelligentDirectorCalibration \endlink API to cancel the calibration at any time.
 * 
 *      2. **How to call the APIs to configure Camera Boundary:**
 *  
 *          1. Camera Boundary is used to configure the main camera's boundary;
 *          2. Call the \link StartCameraBoundaryConfiguration \endlink API to start the Camera Boundary Configuration;
 *          3. When the current configuration action changes, you will receive the \link OnCameraBoundaryConfigurationActionChanged \endlink sink, includes the available actions, call the APIs to continue;
 *          4. After calling the \link CBConfigurationActionAcceptResult \endlink API, Camera Boundary is done. Configure Camera Boundary successfully;
 *          5. Call the \link StopCameraBoundaryConfiguration \endlink API to cancel the configuration at any time.
 * 
 * 
 * ==========================================================================
 * Intelligent Director Calibration workflows
 * ==========================================================================
 * 
 *      1. Calibrate Intelligent Director:
 *          
 *                                                       +--→ IDCalibrationActionAdjustCamera ←------------→ IDCalibrationActionCameraControl
 *                                                       |                  ↕
 *                                                       |    IDCalibrationActionReadyToMove ←---------+        
 *                                                       |                  ↓                          |
 *                                                       +----IDCalibrationActionStartToMove           |
 *                                                                          ↓                          |
 *                             +---------------------------IDCalibrationActionFinishMoveToCheckResult--+
 *                             ↓                                            ↓                       
 *          IDCalibrationActionAcceptCalibrationResult   IDCalibrationActionAskIfNeedSetCameraBoundary ←---------------------+
 *                                                                          ↕                                                |
 *                             +-------------------------→ IDCalibrationActionPreviewIDBoundary                              |
 *                             |                                            ↓                                                |
 *                             |     +------------------------------------------------------------------------------------+  |
 *                             |     | IDCalibrationActionSwitchBoundaryCamera ←→ IDCalibrationActionAdjustCameraBoundary |--+
 *                             |     +------------------------------------------------------------------------------------+
 *                             |                                            ↓
 *                             +--------------------------IDCalibrationActionVerifyBoundaryResults
 *                                                                          ↓
 *                                                         IDCalibrationActionPreAcceptBoundary
 *                                                                          ↓
 *                                                        IDCalibrationActionAcceptBoundaryResult 
 *                                                               
 *      2. Recalibrate Intelligent Director:
 *          
 *          a. if the camera boundary in Intelligent Director Calibration has not been set:
 *      
 *                                                            IDCalibrationActionAdjustCamera -------------→ recalibrate Intelligent Director according to 1. Calibrate Intelligent Director.
 *                                                                          ↓                     
 *                                                       IDCalibrationActionAskIfNeedSetCameraBoundary ←---------------------+
 *                                                                          ↕                                                |
 *                             +-------------------------→ IDCalibrationActionPreviewIDBoundary                              |
 *                             |                                            ↓                                                |
 *                             |     +------------------------------------------------------------------------------------+  |
 *                             |     | IDCalibrationActionSwitchBoundaryCamera ←→ IDCalibrationActionAdjustCameraBoundary |--+
 *                             |     +------------------------------------------------------------------------------------+
 *                             |                                            ↓
 *                             +--------------------------IDCalibrationActionVerifyBoundaryResults
 *                                                                          ↓
 *                                                         IDCalibrationActionPreAcceptBoundary
 *                                                                          ↓
 *                                                        IDCalibrationActionAcceptBoundaryResult 
 * 
 *
 *          b. if the camera boundary in Intelligent Director Calibration has been set:
 *              
 *                         +--------------------------------→ IDCalibrationActionAdjustCamera -------------→ recalibrate Intelligent Director according to 1. Calibrate Intelligent Director.                   
 *                         |                                                ↕
 *                         |                               IDCalibrationActionPreviewIDBoundary ←-----------------------------+                           
 *                         |                                                ↓                                                 |
 *                         |         +------------------------------------------------------------------------------------+   |
 *                         +---------| IDCalibrationActionSwitchBoundaryCamera ←→ IDCalibrationActionAdjustCameraBoundary |   |
 *                                   +------------------------------------------------------------------------------------+   |
 *                                                                          ↓                                                 |
 *                                                        IDCalibrationActionVerifyBoundaryResults----------------------------+
 *                                                                          ↓
 *                                                         IDCalibrationActionPreAcceptBoundary
 *                                                                          ↓
 *                                                        IDCalibrationActionAcceptBoundaryResult
 *
 *
 * ==========================================================================
 * Camera Boundary Configuration workflows
 * ========================================================================== 
 * 
 *      1. Configure Camera Boundary:
 * 
 *                                                         CBConfigurationActionFeatureIntroduce
 *                                                                          ↕
 *                                                      +--→ CBConfigurationActionAdjustCamera ←---------+
 *                                                      |                   ↕                            |
 *                                                      |    CBConfigurationActionReadyToMove ←------+   |
 *                                                      |                   ↓                        |   |
 *                                                      +----CBConfigurationActionStartToMove        |   |
 *                                                                          ↓                        |   |
 *                                                +-------→ CBConfigurationActionGetMoveResult-------+---+
 *                                                |                         ↓                        |
 *                                                |  +-→ CBConfigurationActionAdjustCameraBoundary---+
 *                                                |  |    |                 |
 *                                                |  +----+                 ↓
 *                                                +------CBConfigurationActionVerifyBoundaryResults
 *                                                                          ↓
 *                                                         CBConfigurationActionConfigureFinish
 *                                                                          ↓
 *                                                          CBConfigurationActionAcceptResult 
 * 
 * 
 *      2. Reconfigure Camera Boundary:
 *      
 *                                                         CBConfigurationActionFeatureIntroduce ←------------→ start over Camera Boundary according to 1. Configure Camera Boundary.  
 *                                                                          ↓
 *                           +---------------------------CBConfigurationActionVerifyBoundaryResults
 *                           ↓                                              ↕
 *           CBConfigurationActionConfigureFinish       +--→ CBConfigurationActionAdjustCamera ←---------+
 *                           ↓                          |                   ↕                            |
 *           CBConfigurationActionAcceptResult          |    CBConfigurationActionReadyToMove ←------+   |
 *                                                      |                   ↓                        |   |
 *                                                      +----CBConfigurationActionStartToMove        |   |
 *                                                                          ↓                        |   |
 *                                                +-------→ CBConfigurationActionGetMoveResult-------+---+
 *                                                |                         ↓                        |
 *                                                |  +-→ CBConfigurationActionAdjustCameraBoundary---+
 *                                                |  |    |                 |
 *                                                |  +----+                 ↓
 *                                                +------CBConfigurationActionVerifyBoundaryResults
 *                                                                          ↓
 *                                                         CBConfigurationActionConfigureFinish
 *                                                                          ↓
 *                                                          CBConfigurationActionAcceptResult
 * 
 */
#ifndef ZRC_ICALIBRATION_H
#define ZRC_ICALIBRATION_H

#include "ZRCSDKTypes.h"

NS_ZRCSDK_BEG

/*! \enum IDCalibrationAction, ID(Intelligent Director).
    \brief IDCalibration action type.
    Here are more detailed structural descriptions.
 */
enum class IDCalibrationAction
{
    IDCalibrationActionAdjustCamera,              ///<Adjust camera.
    IDCalibrationActionCameraControl,             ///<Camera control. You can adjust camera position through the \link ICameraControlHelper::ControlLocalCamera \endlink API when you are in this action.
    IDCalibrationActionReadyToMove,               ///<Ready to move.
    IDCalibrationActionStartToMove,               ///<Start to move.
    IDCalibrationActionFinishMoveToCheckResult,   ///<Finish move to check result.
    IDCalibrationActionAcceptCalibrationResult,   ///<Accept calibration result.
    IDCalibrationActionStopCalibration,           ///<Stop calibration.

    // boundary
    IDCalibrationActionAskIfNeedSetCameraBoundary,  ///<Ask if need set camera boundary.
    IDCalibrationActionSwitchBoundaryCamera,        ///<Switch boundary camera.
    IDCalibrationActionAdjustCameraBoundary,        ///<Adjust camera boundary.
    IDCalibrationActionVerifyBoundaryResults,       ///<Verify boundary results.
    IDCalibrationActionPreAcceptBoundary,           ///<Pre-accept boundary result.
    IDCalibrationActionAcceptBoundaryResult,        ///<Accept boundary result.
    IDCalibrationActionPreviewIDBoundary,           ///<Preview ID boundary.
};

/*! \enum CBConfigurationAction, CB(Camera Boundary).
    \brief CBConfigurationAction action.
    Here are more detailed structural descriptions.
 */
enum class CBConfigurationAction
{
    CBConfigurationActionFeatureIntroduce,          ///<Feature introduce.
    CBConfigurationActionAdjustCamera,              ///<Adjust camera. You can adjust camera position through the \link ICameraControlHelper::ControlLocalCamera \endlink API when you are in this action.
    CBConfigurationActionReadyToMove,               ///<Ready to move.
    CBConfigurationActionStartToMove,               ///<Start to move.
    CBConfigurationActionGetMoveResult,             ///<Get move result.
    CBConfigurationActionAdjustCameraBoundary,      ///<Adjust camera boundary.
    CBConfigurationActionVerifyBoundaryResults,     ///<Verify boundary results.
    CBConfigurationActionConfigureFinish,           ///<Configure finish.
    CBConfigurationActionAcceptResult,              ///<Accept result.
    CBConfigurationActionStopConfiguration,         ///<Stop configuration.
};

/*! \enum CameraBoundaryConfigurationResult.
    \brief Camera boundary configuration result.
    Here are more detailed structural descriptions.
 */
enum CameraBoundaryConfigurationResult
{
    CameraBoundaryConfigurationResultUpdateCameraInfo = 0,  ///<Update camera info.
    CameraBoundaryConfigurationResultBoundarySuccess,       ///<Boundary success.
    CameraBoundaryConfigurationResultBoundaryFailed,        ///<Boundary failed.
    CameraBoundaryConfigurationResultBoundaryStop,          ///<Boundary stop.
};

/// \brief Camera boundary configuration notification.
///
struct CameraBoundaryConfigurationNot
{
    CameraBoundaryConfigurationResult result;               ///<Camera boundary configuration result.
    CameraControlInfo                 cameraControlInfo;    ///<Camera control info.
    int32_t                           left = -1;            ///<The value of the left camera boundary.
    int32_t                           right = -1;           ///<The value of the right camera boundary.
    int32_t                           depth = -1;           ///<The value of the depth camera boundary.
};

/// \brief Calibration helper event callback
///
class ICalibrationHelperSink
{
public:
    virtual ~ICalibrationHelperSink() {}

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Director calibration notification.
     *
     * @param noti The notification info.
     */
    virtual void OnDirectorCalibrationNotification(const DirectorCalibrationNot& noti) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Intelligent director calibration action changed notification.
     *
     * @param currentAction The current calibration action.
     * @param actionsOfNextStep The actions of next step in the current.
     * @param actionsOfPreviousStep The actions of previous step in the current.
     */
    virtual void OnIntelligentDirectorCalibrationActionChanged(IDCalibrationAction currentAction, const std::vector<IDCalibrationAction>& actionsOfNextStep, const std::vector<IDCalibrationAction>& actionsOfPreviousStep) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Camera boundary configuration notification.
     *
     * @param noti The notification info.
     */
    virtual void OnCameraBoundaryConfigurationNotification(const CameraBoundaryConfigurationNot& noti) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Camera boundary configuration action changed notification.
     *
     * @param currentAction The current configuration action.
     * @param actionsOfNextStep The actions of next step in the current.
     * @param actionsOfPreviousStep The actions of previous in the current.
     */
    virtual void OnCameraBoundaryConfigurationActionChanged(CBConfigurationAction currentAction, const std::vector<CBConfigurationAction>& actionsOfNextStep, const std::vector<CBConfigurationAction>& actionsOfPreviousStep) = 0;
};

/// \brief Calibration helper interface
///
class ICalibrationHelper
{
public:
    virtual ~ICalibrationHelper() {}

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Register Calibration helper callback sink.
     *
     * @param pSink A pointer to the ICalibrationHelperSink that receives the Calibration helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError RegisterSink(ICalibrationHelperSink* pSink) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Deregister Calibration helper callback sink.
     *
     * @param pSink A pointer to the ICalibrationHelperSink that receives the Calibration helper callback.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError DeregisterSink(ICalibrationHelperSink* pSink) = 0;

    // ==========================================================================
    // Intelligent Director Calibration
    // ==========================================================================

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Select director camera
     *
     * @param deviceID Camera device ID.
     * @param isSelected TRUE indicates to select the camera as director camera.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError SelectIntelligentDirectorCamera(const std::string& deviceID, bool isSelected) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Get Intelligent Director cameras.
     *
     * @param cameras The Intelligent Director cameras.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetIntelligentDirectorCameras(std::vector<Device>& cameras) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Start Intelligent Director calibration.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StartIntelligentDirectorCalibration() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Stop Intelligent Director calibration. You can call this API at any time during the calibration process.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StopIntelligentDirectorCalibration() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Get actions that are able to be done in the current calibration.
     *
     * @param[out] actionsOfNextStep The actions which you can do in the next step.
     * @param[out] actionsOfPreviousStep The actions which you can do in the previous step.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetActionsInCurrentIntelligentDirectorCalibrationStatus(std::vector<IDCalibrationAction>& actionsOfNextStep, std::vector<IDCalibrationAction>& actionsOfPreviousStep) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Adjust camera in ID calibrationAction. The API corresponds to enumeration value IDCalibrationActionAdjustCamera in \link IDCalibrationAction \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IDCalibrationActionAdjustCamera() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Camera control in ID calibrationAction. The API corresponds to enumeration value IDCalibrationActionCameraControl in \link IDCalibrationAction \endlink enum.
     *
     * @param deviceID The camera which will be controlled.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IDCalibrationActionCameraControl(const std::string& deviceID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Ready to move in ID calibrationAction. The API corresponds to enumeration value IDCalibrationActionReadyToMove in \link IDCalibrationAction \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IDCalibrationActionReadyToMove() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Start to move in ID calibrationAction. The API corresponds to enumeration value IDCalibrationActionStartToMove in \link IDCalibrationAction \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IDCalibrationActionStartToMove() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Finish move to check result in ID calibrationAction. The API corresponds to enumeration value IDCalibrationActionFinishMoveToCheckResult in \link IDCalibrationAction \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IDCalibrationActionFinishMoveToCheckResult() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Accept calibration result in ID calibrationAction. The API corresponds to enumeration value IDCalibrationActionAcceptCalibrationResult in \link IDCalibrationAction \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IDCalibrationActionAcceptCalibrationResult() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Ask if need set camera boundary in ID calibrationAction. The API corresponds to enumeration value IDCalibrationActionAskIfNeedSetCameraBoundary in \link IDCalibrationAction \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IDCalibrationActionAskIfNeedSetCameraBoundary() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Switch boundary camera in ID calibrationAction. The API corresponds to enumeration value IDCalibrationActionSwitchBoundaryCamera in \link IDCalibrationAction \endlink enum.
     *
     * @param deviceID The camera ID which will be controlled.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IDCalibrationActionSwitchBoundaryCamera(const std::string& deviceID) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Adjust camera boundary in ID calibrationAction. The API corresponds to enumeration value IDCalibrationActionAdjustCameraBoundary in \link IDCalibrationAction \endlink enum.
     *
     * @param deviceID The camera ID which will be controlled.
     * @param boundaryAdjustField The field which you want to adjust, such as \link CameraBoundaryAdjustField \endlink.
     * @param boundaryAdjustValue The value of the field. The range is [0, 100] when the filed is Left or Right. The range is [50, 150] when the field is Depth.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IDCalibrationActionAdjustCameraBoundary(const std::string& deviceID, CameraBoundaryAdjustField boundaryAdjustField, int32_t boundaryAdjustValue) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Verify boundary results in ID calibrationAction. The API corresponds to enumeration value IDCalibrationActionVerifyBoundaryResults in \link IDCalibrationAction \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IDCalibrationActionVerifyBoundaryResults() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Pre-accept boundary in ID calibrationAction. The API corresponds to enumeration value IDCalibrationActionPreAcceptBoundary in \link IDCalibrationAction \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IDCalibrationActionPreAcceptBoundary() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Accept boundary result in ID calibrationAction. The API corresponds to enumeration value IDCalibrationActionAcceptBoundaryResult in \link IDCalibrationAction \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IDCalibrationActionAcceptBoundaryResult() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Preview ID boundary in ID calibrationAction. The API corresponds to enumeration value IDCalibrationActionPreviewIDBoundary in \link IDCalibrationAction \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError IDCalibrationActionPreviewIDBoundary() = 0;

    // ==========================================================================
    // Camera Boundary Configuration
    // ==========================================================================

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Start Camera Boundary configuration.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StartCameraBoundaryConfiguration() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Stop Camera Boundary configuration.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError StopCameraBoundaryConfiguration() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Get actions that are able to be done in the current Camera Boundary configuration.
     *
     * @param[out] actionsOfNextStep The actions which you can do in the next step.
     * @param[out] actionsOfPreviousStep The actions which you can do in the previous step.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError GetActionsInCurrentCameraBoundaryConfigurationStatus(std::vector<CBConfigurationAction>& actionsOfNextStep, std::vector<CBConfigurationAction>& actionsOfPreviousStep) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Feature introduce in CB configuration. The API corresponds to enumeration value CBConfigurationActionFeatureIntroduce in \link CBConfigurationAction \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CBConfigurationActionFeatureIntroduce() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Adjust camera in CB configuration. The API corresponds to enumeration value CBConfigurationActionAdjustCamera in \link CBConfigurationAction \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CBConfigurationActionAdjustCamera() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Ready to move in CB configuration. The API corresponds to enumeration value CBConfigurationActionReadyToMove in \link CBConfigurationAction \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CBConfigurationActionReadyToMove() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Start to move in CB configuration. The API corresponds to enumeration value CBConfigurationActionStartToMove in \link CBConfigurationAction \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CBConfigurationActionStartToMove() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Get move result in CB configuration. The API corresponds to enumeration value CBConfigurationActionGetMoveResult in \link CBConfigurationAction \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CBConfigurationActionGetMoveResult() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Adjust camera boundary in CB configuration. The API corresponds to enumeration value CBConfigurationActionAdjustCameraBoundary in \link CBConfigurationAction \endlink enum.
     *
     * @param boundaryAdjustField The field which you want to adjust, such as \link CameraBoundaryAdjustField \endlink.
     * @param boundaryAdjustValue The value of the field. The range is [0, 100] when the filed is Left or Right. The range is [50, 150] when the field is Depth.
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CBConfigurationActionAdjustCameraBoundary(CameraBoundaryAdjustField boundaryAdjustField, int32_t boundaryAdjustValue) = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Verify boundary results in CB configuration. The API corresponds to enumeration value CBConfigurationActionVerifyBoundaryResults in \link CBConfigurationAction \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CBConfigurationActionVerifyBoundaryResults() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Configure finish in CB configuration. The API corresponds to enumeration value CBConfigurationActionConfigureFinish in \link CBConfigurationAction \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CBConfigurationActionConfigureFinish() = 0;

    /**
     * @since 6.1.0
     * @zrminver 6.1.0
     *
     * @brief Accept result in CB configuration. The API corresponds to enumeration value CBConfigurationActionAcceptResult in \link CBConfigurationAction \endlink enum.
     *
     * @return If the function succeeds, the return value is @ref ZRCSDKERR_SUCCESS.
     * Otherwise failed. To get extended error information, see \link ZRCSDKError \endlink enum.
     */
    virtual ZRCSDKError CBConfigurationActionAcceptResult() = 0;
};

NS_ZRCSDK_END

#endif    //ZRC_ICALIBRATION_H
