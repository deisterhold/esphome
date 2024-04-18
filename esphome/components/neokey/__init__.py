import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, i2c, light
from esphome.const import (
    CONF_OUTPUT_ID,
)

AUTO_LOAD = ["binary_sensor", "light"]
CODEOWNERS = ["@deisterhold"]
DEPENDENCIES = ["i2c"]
MULTI_CONF = True

neokey_ns = cg.esphome_ns.namespace("neokey")
NeoKeyComponent = neokey_ns.class_(
    "NeoKeyComponent", light.AddressableLight, i2c.I2CDevice
)


CONF_KEY_1 = "key_1"
CONF_KEY_2 = "key_2"
CONF_KEY_3 = "key_3"
CONF_KEY_4 = "key_4"


CONFIG_SCHEMA = light.ADDRESSABLE_LIGHT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(NeoKeyComponent),
        cv.Required(CONF_KEY_1): binary_sensor.binary_sensor_schema(),
        cv.Required(CONF_KEY_2): binary_sensor.binary_sensor_schema(),
        cv.Required(CONF_KEY_3): binary_sensor.binary_sensor_schema(),
        cv.Required(CONF_KEY_4): binary_sensor.binary_sensor_schema(),
    }
).extend(i2c.i2c_device_schema(0x30))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await light.register_light(var, config)
    await i2c.register_i2c_device(var, config)
    await cg.register_component(var, config)

    bs1 = await binary_sensor.new_binary_sensor(config[CONF_KEY_1])
    cg.add(var.set_key_1_sensor(bs1))

    bs2 = await binary_sensor.new_binary_sensor(config[CONF_KEY_2])
    cg.add(var.set_key_2_sensor(bs2))

    bs3 = await binary_sensor.new_binary_sensor(config[CONF_KEY_3])
    cg.add(var.set_key_3_sensor(bs3))

    bs4 = await binary_sensor.new_binary_sensor(config[CONF_KEY_4])
    cg.add(var.set_key_4_sensor(bs4))

    cg.add_library("SPI", None)
    cg.add_library("Wire", None)
    cg.add_library("adafruit/Adafruit BusIO", None)
    cg.add_library("adafruit/Adafruit seesaw Library", None)
