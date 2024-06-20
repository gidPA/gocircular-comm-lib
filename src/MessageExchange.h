#ifndef MESSAGE_EXCHANGE_H
#define MESSAGE_EXCHANGE_H

#include <Arduino.h>

#define MESSAGE_SIZE 10
#define MESSAGE_START_CODE 252
#define MESSAGE_END_CODE 254

/**
 * @enum messageTopic
 * @brief Enumeration of different message topics.
 */
enum messageTopic
{
    TOPIC_ERR,             ///< Placeholder for 0, reserved for future use.
    BEGIN_TRANSACTION,     ///< Begin transaction, no payloads.
    ITEM_ENTRY,            ///< An item is entered, 4 payloads (status, material, size, points granted).
    SET_MEMBER_MODE,       ///< Specify whether the following transaction is going to run with a companion app, no payload.
    SET_EXCHANGE_RATE,     ///< Set exchange rate, 2 payloads.
    READY_FOR_TRANSACTION, ///< Tell IoT gateway that system is ready for transaction, no payload.
    TRANSACTION_COMPLETE,  ///< Finish transaction, no payloads.
    BIN_FULL,              ///< Trigger an alert to be sent the server that this particular bin is full, 1 payload (specifies the full bin).
    COIN_DISPENSER_ALERT   ///< Trigger an alert to be sent to the server that the coin dispenser has run out of coins.
};

/**
 * @enum itemType
 * @brief Enumeration of different item types.
 */
enum itemType
{
    ITEM_ERR,              ///< Error or unknown item type.
    PLASTIC_COLOURED,      ///< Colored plastic bottle.
    PLASTIC_TRANSPARENT,   ///< Transparent plastic bottle.
    METAL                  ///< Metallic can.
};

/**
 * @enum itemSize
 * @brief Enumeration of different item sizes.
 */
enum itemSize
{
    SIZE_ERR, ///< Error or unknown size.
    SMALL,    ///< Small size.
    MEDIUM,   ///< Medium size.
    LARGE     ///< Large size.
};

/**
 * @enum itemStatus
 * @brief Enumeration of different item statuses.
 */
enum itemStatus
{
    STAT_ERR, ///< Error or unknown status.
    DECLINED, ///< Item entry declined.
    ACCEPTED, ///< Item entry accepted.
    PENDING   ///< Item entry pending.
};

/**
 * @class MessageExchange
 * @brief Class for managing fixed-length topic-based UART communication.
 */
class MessageExchange
{
private:
    byte message[MESSAGE_SIZE]; ///< Array to store the message data.

    const char *getMessageTopicName(messageTopic topic);
    const char *getItemTypeName(itemType type);
    const char *getItemSizeName(itemSize size);
    const char *getItemStatusName(itemStatus status);

    void clearSerialBuffer();

public:
    HardwareSerial *uartDevice; ///< UART device for communication.
    HardwareSerial *uartMonitoringDevice; ///< UART device for monitoring.

    /**
     * @brief Constructor for MessageExchange.
     */
    MessageExchange();

    /**
     * @brief Set the UART device for communication.
     * @param device Pointer to the HardwareSerial device.
     */
    void setUartDevice(HardwareSerial *device);

    /**
     * @brief Set the UART device for monitoring.
     * @param device Pointer to the HardwareSerial device.
     */
    void setUartMonitoringDevice(HardwareSerial *device);

    /**
     * @brief Create a new message with a specified topic.
     * @param topic The message topic.
     */
    void createNewMessage(messageTopic topic);

    /**
     * @brief Set a specific byte in the message.
     * @param bytePosition The position of the byte.
     * @param byteData The data to set.
     */
    void setData(byte bytePosition, byte byteData);

    /**
     * @brief Get a specific byte from the message.
     * @param byteNum The position of the byte.
     * @return The byte data.
     */
    byte getData(byte byteNum);

    /**
     * @brief Set the item entry status in the message.
     * @param status The item status.
     */
    void setItemEntryStatus(itemStatus status);

    /**
     * @brief Get the item entry status from the message.
     * @return The item status.
     */
    byte getItemEntryStatus();

    /**
     * @brief Set the item type in the message.
     * @param type The item type.
     */
    void setItemType(itemType type);

    /**
     * @brief Get the item type from the message.
     * @return The item type.
     */
    byte getItemType();

    /**
     * @brief Set the item size in the message.
     * @param size The item size.
     */
    void setItemSize(itemSize size);

    /**
     * @brief Get the item size from the message.
     * @return The item size.
     */
    byte getItemSize();

    /**
     * @brief Set the item points in the message.
     * @param point The item points.
     */
    void setItemPoint(byte point);

    /**
     * @brief Get the item points from the message.
     * @return The item points.
     */
    byte getItemPoint();

    /**
     * @brief Preview the message data for debugging purposes.
     */
    void previewMessage();

    /**
     * @brief Send the message via UART.
     */
    void sendMessage();

    /**
     * @brief Handle an incoming message via UART.
     * @return The message topic.
     */
    byte handleIncomingMessage();
};

#endif // MESSAGE_EXCHANGE_H