// Copyright 2020-2022 The MathWorks, Inc.
// Common copy functions for boro_interfaces/BowEvent
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100)
#pragma warning(disable : 4265)
#pragma warning(disable : 4456)
#pragma warning(disable : 4458)
#pragma warning(disable : 4946)
#pragma warning(disable : 4244)
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wredundant-decls"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif //_MSC_VER
#include "rclcpp/rclcpp.hpp"
#include "boro_interfaces/msg/bow_event.hpp"
#include "visibility_control.h"
#include "class_loader/multi_library_class_loader.hpp"
#include "ROS2PubSubTemplates.hpp"
class BORO_INTERFACES_EXPORT ros2_boro_interfaces_msg_BowEvent_common : public MATLABROS2MsgInterface<boro_interfaces::msg::BowEvent> {
  public:
    virtual ~ros2_boro_interfaces_msg_BowEvent_common(){}
    virtual void copy_from_struct(boro_interfaces::msg::BowEvent* msg, const matlab::data::Struct& arr, MultiLibLoader loader); 
    //----------------------------------------------------------------------------
    virtual MDArray_T get_arr(MDFactory_T& factory, const boro_interfaces::msg::BowEvent* msg, MultiLibLoader loader, size_t size = 1);
};
  void ros2_boro_interfaces_msg_BowEvent_common::copy_from_struct(boro_interfaces::msg::BowEvent* msg, const matlab::data::Struct& arr,
               MultiLibLoader loader) {
    try {
        //header
        const matlab::data::StructArray header_arr = arr["header"];
        auto msgClassPtr_header = getCommonObject<std_msgs::msg::Header>("ros2_std_msgs_msg_Header_common",loader);
        msgClassPtr_header->copy_from_struct(&msg->header,header_arr[0],loader);
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'header' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'header' is wrong type; expected a struct.");
    }
    try {
        //event_code
        const matlab::data::TypedArray<uint32_t> event_code_arr = arr["event_code"];
        msg->event_code = event_code_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'event_code' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'event_code' is wrong type; expected a uint32.");
    }
    try {
        //state_name
        const matlab::data::CharArray state_name_arr = arr["state_name"];
        msg->state_name = state_name_arr.toAscii();
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'state_name' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'state_name' is wrong type; expected a string.");
    }
    try {
        //description
        const matlab::data::CharArray description_arr = arr["description"];
        msg->description = description_arr.toAscii();
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'description' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'description' is wrong type; expected a string.");
    }
    try {
        //level_error
        const matlab::data::TypedArray<bool> level_error_arr = arr["level_error"];
        msg->level_error = level_error_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'level_error' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'level_error' is wrong type; expected a logical.");
    }
  }
  //----------------------------------------------------------------------------
  MDArray_T ros2_boro_interfaces_msg_BowEvent_common::get_arr(MDFactory_T& factory, const boro_interfaces::msg::BowEvent* msg,
       MultiLibLoader loader, size_t size) {
    auto outArray = factory.createStructArray({size,1},{"MessageType","header","NONE","READY","HOME_REACHED","CONFIG_SELECTED","ANGLE_REACHED","CONTACT_REACHED","CONTACT_READY","CONFIG_PLAYED","BOW_RAISED","SHUTDOWN_SELECTED","ERROR","CONTACT_LOST","NO_CONTACT_DETECTED","TIMEOUT","event_code","state_name","description","level_error"});
    for(size_t ctr = 0; ctr < size; ctr++){
    outArray[ctr]["MessageType"] = factory.createCharArray("boro_interfaces/BowEvent");
    // header
    auto currentElement_header = (msg + ctr)->header;
    auto msgClassPtr_header = getCommonObject<std_msgs::msg::Header>("ros2_std_msgs_msg_Header_common",loader);
    outArray[ctr]["header"] = msgClassPtr_header->get_arr(factory, &currentElement_header, loader);
    // NONE
    auto currentElement_NONE = (msg + ctr)->NONE;
    outArray[ctr]["NONE"] = factory.createScalar(currentElement_NONE);
    // READY
    auto currentElement_READY = (msg + ctr)->READY;
    outArray[ctr]["READY"] = factory.createScalar(currentElement_READY);
    // HOME_REACHED
    auto currentElement_HOME_REACHED = (msg + ctr)->HOME_REACHED;
    outArray[ctr]["HOME_REACHED"] = factory.createScalar(currentElement_HOME_REACHED);
    // CONFIG_SELECTED
    auto currentElement_CONFIG_SELECTED = (msg + ctr)->CONFIG_SELECTED;
    outArray[ctr]["CONFIG_SELECTED"] = factory.createScalar(currentElement_CONFIG_SELECTED);
    // ANGLE_REACHED
    auto currentElement_ANGLE_REACHED = (msg + ctr)->ANGLE_REACHED;
    outArray[ctr]["ANGLE_REACHED"] = factory.createScalar(currentElement_ANGLE_REACHED);
    // CONTACT_REACHED
    auto currentElement_CONTACT_REACHED = (msg + ctr)->CONTACT_REACHED;
    outArray[ctr]["CONTACT_REACHED"] = factory.createScalar(currentElement_CONTACT_REACHED);
    // CONTACT_READY
    auto currentElement_CONTACT_READY = (msg + ctr)->CONTACT_READY;
    outArray[ctr]["CONTACT_READY"] = factory.createScalar(currentElement_CONTACT_READY);
    // CONFIG_PLAYED
    auto currentElement_CONFIG_PLAYED = (msg + ctr)->CONFIG_PLAYED;
    outArray[ctr]["CONFIG_PLAYED"] = factory.createScalar(currentElement_CONFIG_PLAYED);
    // BOW_RAISED
    auto currentElement_BOW_RAISED = (msg + ctr)->BOW_RAISED;
    outArray[ctr]["BOW_RAISED"] = factory.createScalar(currentElement_BOW_RAISED);
    // SHUTDOWN_SELECTED
    auto currentElement_SHUTDOWN_SELECTED = (msg + ctr)->SHUTDOWN_SELECTED;
    outArray[ctr]["SHUTDOWN_SELECTED"] = factory.createScalar(currentElement_SHUTDOWN_SELECTED);
    // ERROR
    auto currentElement_ERROR = (msg + ctr)->ERROR;
    outArray[ctr]["ERROR"] = factory.createScalar(currentElement_ERROR);
    // CONTACT_LOST
    auto currentElement_CONTACT_LOST = (msg + ctr)->CONTACT_LOST;
    outArray[ctr]["CONTACT_LOST"] = factory.createScalar(currentElement_CONTACT_LOST);
    // NO_CONTACT_DETECTED
    auto currentElement_NO_CONTACT_DETECTED = (msg + ctr)->NO_CONTACT_DETECTED;
    outArray[ctr]["NO_CONTACT_DETECTED"] = factory.createScalar(currentElement_NO_CONTACT_DETECTED);
    // TIMEOUT
    auto currentElement_TIMEOUT = (msg + ctr)->TIMEOUT;
    outArray[ctr]["TIMEOUT"] = factory.createScalar(currentElement_TIMEOUT);
    // event_code
    auto currentElement_event_code = (msg + ctr)->event_code;
    outArray[ctr]["event_code"] = factory.createScalar(currentElement_event_code);
    // state_name
    auto currentElement_state_name = (msg + ctr)->state_name;
    outArray[ctr]["state_name"] = factory.createCharArray(currentElement_state_name);
    // description
    auto currentElement_description = (msg + ctr)->description;
    outArray[ctr]["description"] = factory.createCharArray(currentElement_description);
    // level_error
    auto currentElement_level_error = (msg + ctr)->level_error;
    outArray[ctr]["level_error"] = factory.createScalar(currentElement_level_error);
    }
    return std::move(outArray);
  } 
class BORO_INTERFACES_EXPORT ros2_boro_interfaces_BowEvent_message : public ROS2MsgElementInterfaceFactory {
  public:
    virtual ~ros2_boro_interfaces_BowEvent_message(){}
    virtual std::shared_ptr<MATLABPublisherInterface> generatePublisherInterface(ElementType /*type*/);
    virtual std::shared_ptr<MATLABSubscriberInterface> generateSubscriberInterface(ElementType /*type*/);
    virtual std::shared_ptr<void> generateCppMessage(ElementType /*type*/, const matlab::data::StructArray& /* arr */, MultiLibLoader /* loader */, std::map<std::string,std::shared_ptr<MATLABROS2MsgInterfaceBase>>* /*commonObjMap*/);
    virtual matlab::data::StructArray generateMLMessage(ElementType  /*type*/ ,void*  /* msg */, MultiLibLoader /* loader */ , std::map<std::string,std::shared_ptr<MATLABROS2MsgInterfaceBase>>* /*commonObjMap*/);
};  
  std::shared_ptr<MATLABPublisherInterface> 
          ros2_boro_interfaces_BowEvent_message::generatePublisherInterface(ElementType /*type*/){
    return std::make_shared<ROS2PublisherImpl<boro_interfaces::msg::BowEvent,ros2_boro_interfaces_msg_BowEvent_common>>();
  }
  std::shared_ptr<MATLABSubscriberInterface> 
         ros2_boro_interfaces_BowEvent_message::generateSubscriberInterface(ElementType /*type*/){
    return std::make_shared<ROS2SubscriberImpl<boro_interfaces::msg::BowEvent,ros2_boro_interfaces_msg_BowEvent_common>>();
  }
  std::shared_ptr<void> ros2_boro_interfaces_BowEvent_message::generateCppMessage(ElementType /*type*/, 
                                           const matlab::data::StructArray& arr,
                                           MultiLibLoader loader,
                                           std::map<std::string,std::shared_ptr<MATLABROS2MsgInterfaceBase>>* commonObjMap){
    auto msg = std::make_shared<boro_interfaces::msg::BowEvent>();
    ros2_boro_interfaces_msg_BowEvent_common commonObj;
    commonObj.mCommonObjMap = commonObjMap;
    commonObj.copy_from_struct(msg.get(), arr[0], loader);
    return msg;
  }
  matlab::data::StructArray ros2_boro_interfaces_BowEvent_message::generateMLMessage(ElementType  /*type*/ ,
                                                    void*  msg ,
                                                    MultiLibLoader  loader ,
                                                    std::map<std::string,std::shared_ptr<MATLABROS2MsgInterfaceBase>>*  commonObjMap ){
    ros2_boro_interfaces_msg_BowEvent_common commonObj;	
    commonObj.mCommonObjMap = commonObjMap;	
    MDFactory_T factory;
    return commonObj.get_arr(factory, (boro_interfaces::msg::BowEvent*)msg, loader);			
 }
#include "class_loader/register_macro.hpp"
// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
CLASS_LOADER_REGISTER_CLASS(ros2_boro_interfaces_msg_BowEvent_common, MATLABROS2MsgInterface<boro_interfaces::msg::BowEvent>)
CLASS_LOADER_REGISTER_CLASS(ros2_boro_interfaces_BowEvent_message, ROS2MsgElementInterfaceFactory)
#ifdef _MSC_VER
#pragma warning(pop)
#else
#pragma GCC diagnostic pop
#endif //_MSC_VER