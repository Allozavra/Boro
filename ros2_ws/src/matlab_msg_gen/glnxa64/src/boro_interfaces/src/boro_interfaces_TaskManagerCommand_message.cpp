// Copyright 2020-2022 The MathWorks, Inc.
// Common copy functions for boro_interfaces/TaskManagerCommand
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
#include "boro_interfaces/msg/task_manager_command.hpp"
#include "visibility_control.h"
#include "class_loader/multi_library_class_loader.hpp"
#include "ROS2PubSubTemplates.hpp"
class BORO_INTERFACES_EXPORT ros2_boro_interfaces_msg_TaskManagerCommand_common : public MATLABROS2MsgInterface<boro_interfaces::msg::TaskManagerCommand> {
  public:
    virtual ~ros2_boro_interfaces_msg_TaskManagerCommand_common(){}
    virtual void copy_from_struct(boro_interfaces::msg::TaskManagerCommand* msg, const matlab::data::Struct& arr, MultiLibLoader loader); 
    //----------------------------------------------------------------------------
    virtual MDArray_T get_arr(MDFactory_T& factory, const boro_interfaces::msg::TaskManagerCommand* msg, MultiLibLoader loader, size_t size = 1);
};
  void ros2_boro_interfaces_msg_TaskManagerCommand_common::copy_from_struct(boro_interfaces::msg::TaskManagerCommand* msg, const matlab::data::Struct& arr,
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
        //command
        const matlab::data::TypedArray<uint8_t> command_arr = arr["command"];
        msg->command = command_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'command' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'command' is wrong type; expected a uint8.");
    }
    try {
        //selected_string
        const matlab::data::CharArray selected_string_arr = arr["selected_string"];
        msg->selected_string = selected_string_arr.toAscii();
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'selected_string' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'selected_string' is wrong type; expected a string.");
    }
    try {
        //desired_alpha
        const matlab::data::TypedArray<double> desired_alpha_arr = arr["desired_alpha"];
        msg->desired_alpha = desired_alpha_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'desired_alpha' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'desired_alpha' is wrong type; expected a double.");
    }
    try {
        //desired_force
        const matlab::data::TypedArray<double> desired_force_arr = arr["desired_force"];
        msg->desired_force = desired_force_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'desired_force' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'desired_force' is wrong type; expected a double.");
    }
    try {
        //bow_velocity
        const matlab::data::TypedArray<double> bow_velocity_arr = arr["bow_velocity"];
        msg->bow_velocity = bow_velocity_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'bow_velocity' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'bow_velocity' is wrong type; expected a double.");
    }
    try {
        //bow_range_min
        const matlab::data::TypedArray<double> bow_range_min_arr = arr["bow_range_min"];
        msg->bow_range_min = bow_range_min_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'bow_range_min' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'bow_range_min' is wrong type; expected a double.");
    }
    try {
        //bow_range_max
        const matlab::data::TypedArray<double> bow_range_max_arr = arr["bow_range_max"];
        msg->bow_range_max = bow_range_max_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'bow_range_max' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'bow_range_max' is wrong type; expected a double.");
    }
    try {
        //h_approach_velocity
        const matlab::data::TypedArray<double> h_approach_velocity_arr = arr["h_approach_velocity"];
        msg->h_approach_velocity = h_approach_velocity_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'h_approach_velocity' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'h_approach_velocity' is wrong type; expected a double.");
    }
    try {
        //h_raise_velocity
        const matlab::data::TypedArray<double> h_raise_velocity_arr = arr["h_raise_velocity"];
        msg->h_raise_velocity = h_raise_velocity_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'h_raise_velocity' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'h_raise_velocity' is wrong type; expected a double.");
    }
    try {
        //auto_continue
        const matlab::data::TypedArray<bool> auto_continue_arr = arr["auto_continue"];
        msg->auto_continue = auto_continue_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'auto_continue' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'auto_continue' is wrong type; expected a logical.");
    }
  }
  //----------------------------------------------------------------------------
  MDArray_T ros2_boro_interfaces_msg_TaskManagerCommand_common::get_arr(MDFactory_T& factory, const boro_interfaces::msg::TaskManagerCommand* msg,
       MultiLibLoader loader, size_t size) {
    auto outArray = factory.createStructArray({size,1},{"MessageType","header","CMD_NONE","CMD_INIT","CMD_HOME","CMD_SELECT_CONFIG","CMD_ROTATE","CMD_LOWER_BOW","CMD_BOWING","CMD_RAISE_BOW","CMD_FINISH","CMD_ERROR","command","selected_string","desired_alpha","desired_force","bow_velocity","bow_range_min","bow_range_max","h_approach_velocity","h_raise_velocity","auto_continue"});
    for(size_t ctr = 0; ctr < size; ctr++){
    outArray[ctr]["MessageType"] = factory.createCharArray("boro_interfaces/TaskManagerCommand");
    // header
    auto currentElement_header = (msg + ctr)->header;
    auto msgClassPtr_header = getCommonObject<std_msgs::msg::Header>("ros2_std_msgs_msg_Header_common",loader);
    outArray[ctr]["header"] = msgClassPtr_header->get_arr(factory, &currentElement_header, loader);
    // CMD_NONE
    auto currentElement_CMD_NONE = (msg + ctr)->CMD_NONE;
    outArray[ctr]["CMD_NONE"] = factory.createScalar(currentElement_CMD_NONE);
    // CMD_INIT
    auto currentElement_CMD_INIT = (msg + ctr)->CMD_INIT;
    outArray[ctr]["CMD_INIT"] = factory.createScalar(currentElement_CMD_INIT);
    // CMD_HOME
    auto currentElement_CMD_HOME = (msg + ctr)->CMD_HOME;
    outArray[ctr]["CMD_HOME"] = factory.createScalar(currentElement_CMD_HOME);
    // CMD_SELECT_CONFIG
    auto currentElement_CMD_SELECT_CONFIG = (msg + ctr)->CMD_SELECT_CONFIG;
    outArray[ctr]["CMD_SELECT_CONFIG"] = factory.createScalar(currentElement_CMD_SELECT_CONFIG);
    // CMD_ROTATE
    auto currentElement_CMD_ROTATE = (msg + ctr)->CMD_ROTATE;
    outArray[ctr]["CMD_ROTATE"] = factory.createScalar(currentElement_CMD_ROTATE);
    // CMD_LOWER_BOW
    auto currentElement_CMD_LOWER_BOW = (msg + ctr)->CMD_LOWER_BOW;
    outArray[ctr]["CMD_LOWER_BOW"] = factory.createScalar(currentElement_CMD_LOWER_BOW);
    // CMD_BOWING
    auto currentElement_CMD_BOWING = (msg + ctr)->CMD_BOWING;
    outArray[ctr]["CMD_BOWING"] = factory.createScalar(currentElement_CMD_BOWING);
    // CMD_RAISE_BOW
    auto currentElement_CMD_RAISE_BOW = (msg + ctr)->CMD_RAISE_BOW;
    outArray[ctr]["CMD_RAISE_BOW"] = factory.createScalar(currentElement_CMD_RAISE_BOW);
    // CMD_FINISH
    auto currentElement_CMD_FINISH = (msg + ctr)->CMD_FINISH;
    outArray[ctr]["CMD_FINISH"] = factory.createScalar(currentElement_CMD_FINISH);
    // CMD_ERROR
    auto currentElement_CMD_ERROR = (msg + ctr)->CMD_ERROR;
    outArray[ctr]["CMD_ERROR"] = factory.createScalar(currentElement_CMD_ERROR);
    // command
    auto currentElement_command = (msg + ctr)->command;
    outArray[ctr]["command"] = factory.createScalar(currentElement_command);
    // selected_string
    auto currentElement_selected_string = (msg + ctr)->selected_string;
    outArray[ctr]["selected_string"] = factory.createCharArray(currentElement_selected_string);
    // desired_alpha
    auto currentElement_desired_alpha = (msg + ctr)->desired_alpha;
    outArray[ctr]["desired_alpha"] = factory.createScalar(currentElement_desired_alpha);
    // desired_force
    auto currentElement_desired_force = (msg + ctr)->desired_force;
    outArray[ctr]["desired_force"] = factory.createScalar(currentElement_desired_force);
    // bow_velocity
    auto currentElement_bow_velocity = (msg + ctr)->bow_velocity;
    outArray[ctr]["bow_velocity"] = factory.createScalar(currentElement_bow_velocity);
    // bow_range_min
    auto currentElement_bow_range_min = (msg + ctr)->bow_range_min;
    outArray[ctr]["bow_range_min"] = factory.createScalar(currentElement_bow_range_min);
    // bow_range_max
    auto currentElement_bow_range_max = (msg + ctr)->bow_range_max;
    outArray[ctr]["bow_range_max"] = factory.createScalar(currentElement_bow_range_max);
    // h_approach_velocity
    auto currentElement_h_approach_velocity = (msg + ctr)->h_approach_velocity;
    outArray[ctr]["h_approach_velocity"] = factory.createScalar(currentElement_h_approach_velocity);
    // h_raise_velocity
    auto currentElement_h_raise_velocity = (msg + ctr)->h_raise_velocity;
    outArray[ctr]["h_raise_velocity"] = factory.createScalar(currentElement_h_raise_velocity);
    // auto_continue
    auto currentElement_auto_continue = (msg + ctr)->auto_continue;
    outArray[ctr]["auto_continue"] = factory.createScalar(currentElement_auto_continue);
    }
    return std::move(outArray);
  } 
class BORO_INTERFACES_EXPORT ros2_boro_interfaces_TaskManagerCommand_message : public ROS2MsgElementInterfaceFactory {
  public:
    virtual ~ros2_boro_interfaces_TaskManagerCommand_message(){}
    virtual std::shared_ptr<MATLABPublisherInterface> generatePublisherInterface(ElementType /*type*/);
    virtual std::shared_ptr<MATLABSubscriberInterface> generateSubscriberInterface(ElementType /*type*/);
    virtual std::shared_ptr<void> generateCppMessage(ElementType /*type*/, const matlab::data::StructArray& /* arr */, MultiLibLoader /* loader */, std::map<std::string,std::shared_ptr<MATLABROS2MsgInterfaceBase>>* /*commonObjMap*/);
    virtual matlab::data::StructArray generateMLMessage(ElementType  /*type*/ ,void*  /* msg */, MultiLibLoader /* loader */ , std::map<std::string,std::shared_ptr<MATLABROS2MsgInterfaceBase>>* /*commonObjMap*/);
};  
  std::shared_ptr<MATLABPublisherInterface> 
          ros2_boro_interfaces_TaskManagerCommand_message::generatePublisherInterface(ElementType /*type*/){
    return std::make_shared<ROS2PublisherImpl<boro_interfaces::msg::TaskManagerCommand,ros2_boro_interfaces_msg_TaskManagerCommand_common>>();
  }
  std::shared_ptr<MATLABSubscriberInterface> 
         ros2_boro_interfaces_TaskManagerCommand_message::generateSubscriberInterface(ElementType /*type*/){
    return std::make_shared<ROS2SubscriberImpl<boro_interfaces::msg::TaskManagerCommand,ros2_boro_interfaces_msg_TaskManagerCommand_common>>();
  }
  std::shared_ptr<void> ros2_boro_interfaces_TaskManagerCommand_message::generateCppMessage(ElementType /*type*/, 
                                           const matlab::data::StructArray& arr,
                                           MultiLibLoader loader,
                                           std::map<std::string,std::shared_ptr<MATLABROS2MsgInterfaceBase>>* commonObjMap){
    auto msg = std::make_shared<boro_interfaces::msg::TaskManagerCommand>();
    ros2_boro_interfaces_msg_TaskManagerCommand_common commonObj;
    commonObj.mCommonObjMap = commonObjMap;
    commonObj.copy_from_struct(msg.get(), arr[0], loader);
    return msg;
  }
  matlab::data::StructArray ros2_boro_interfaces_TaskManagerCommand_message::generateMLMessage(ElementType  /*type*/ ,
                                                    void*  msg ,
                                                    MultiLibLoader  loader ,
                                                    std::map<std::string,std::shared_ptr<MATLABROS2MsgInterfaceBase>>*  commonObjMap ){
    ros2_boro_interfaces_msg_TaskManagerCommand_common commonObj;	
    commonObj.mCommonObjMap = commonObjMap;	
    MDFactory_T factory;
    return commonObj.get_arr(factory, (boro_interfaces::msg::TaskManagerCommand*)msg, loader);			
 }
#include "class_loader/register_macro.hpp"
// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
CLASS_LOADER_REGISTER_CLASS(ros2_boro_interfaces_msg_TaskManagerCommand_common, MATLABROS2MsgInterface<boro_interfaces::msg::TaskManagerCommand>)
CLASS_LOADER_REGISTER_CLASS(ros2_boro_interfaces_TaskManagerCommand_message, ROS2MsgElementInterfaceFactory)
#ifdef _MSC_VER
#pragma warning(pop)
#else
#pragma GCC diagnostic pop
#endif //_MSC_VER