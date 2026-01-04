import esphome.codegen as cg
from esphome.components import pn532, uart
import esphome.config_validation as cv
from esphome.const import CONF_ID

AUTO_LOAD = ["pn532"]
CODEOWNERS = ["@deisterhold"]
DEPENDENCIES = ["uart"]
MULTI_CONF = True

pn532_uart_ns = cg.esphome_ns.namespace("pn532_uart")
PN532UART = pn532_uart_ns.class_("PN532UART", pn532.PN532, uart.UARTDevice)

CONFIG_SCHEMA = cv.All(
    pn532.PN532_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(PN532UART),
        }
    ).extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await pn532.setup_pn532(var, config)
    await uart.register_uart_device(var, config)
