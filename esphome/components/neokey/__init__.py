import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import (
    CONF_ADDRESS,
    CONF_ID,
)

AUTO_LOAD = ["binary_sensor"]
DEPENDENCIES = ["i2c"]

neokey_ns = cg.esphome_ns.namespace("neokey")
NeoKeyComponent = neokey_ns.class_("NeoKeyComponent", cg.Component)


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(NeoKeyComponent),
            cv.Optional(CONF_ADDRESS, default=0x30): cv.i2c_address,
        }
    ).extend(cv.COMPONENT_SCHEMA),
)


async def to_code(config):
    neokey = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(neokey, config)

    cg.add(neokey.set_address(config[CONF_ADDRESS]))

    cg.add_library("SPI", None)
    cg.add_library("Wire", None)
    cg.add_library("adafruit/Adafruit BusIO", None)
    cg.add_library("adafruit/Adafruit seesaw Library", None)
