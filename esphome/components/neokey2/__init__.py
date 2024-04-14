import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, i2c
from esphome.const import (
    CONF_ID,
)

CODEOWNERS = ["@deisterhold"]
DEPENDENCIES = ["binary_sensor", "i2c"]

neokey2_ns = cg.esphome_ns.namespace("neokey2")
NeoKey2Component = neokey2_ns.class_("NeoKey2Component", cg.Component, i2c.I2CDevice)


CONF_KEY_1 = "key_1"
CONF_KEY_2 = "key_2"
CONF_KEY_3 = "key_3"
CONF_KEY_4 = "key_4"


CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(NeoKey2Component),
            cv.Optional(CONF_KEY_1): binary_sensor.binary_sensor_schema(),
            cv.Optional(CONF_KEY_2): binary_sensor.binary_sensor_schema(),
            cv.Optional(CONF_KEY_3): binary_sensor.binary_sensor_schema(),
            cv.Optional(CONF_KEY_4): binary_sensor.binary_sensor_schema(),
        },
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x30))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if CONF_KEY_1 in config:
        bs = await binary_sensor.new_binary_sensor(config[CONF_KEY_1])
        cg.add(var.set_key_1_sensor(bs))
    if CONF_KEY_2 in config:
        bs = await binary_sensor.new_binary_sensor(config[CONF_KEY_2])
        cg.add(var.set_key_2_sensor(bs))
    if CONF_KEY_3 in config:
        bs = await binary_sensor.new_binary_sensor(config[CONF_KEY_3])
        cg.add(var.set_key_3_sensor(bs))
    if CONF_KEY_4 in config:
        bs = await binary_sensor.new_binary_sensor(config[CONF_KEY_4])
        cg.add(var.set_key_4_sensor(bs))

    cg.add_library("SPI", None)
    cg.add_library("Wire", None)
    cg.add_library("adafruit/Adafruit BusIO", None)
    cg.add_library("adafruit/Adafruit seesaw Library", None)
