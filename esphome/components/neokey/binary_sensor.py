import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ADDRESS

CODEOWNERS = ["@deisterhold"]

DEPENDENCIES = ["i2c"]

neokey_ns = cg.esphome_ns.namespace("neokey")

NeoKeyBinarySensor = neokey_ns.class_(
    "NeoKeyBinarySensor", binary_sensor.BinarySensor, cg.Component
)

CONFIG_SCHEMA = (
    binary_sensor.binary_sensor_schema(NeoKeyBinarySensor)
    .extend({cv.Optional(CONF_ADDRESS, default=0x30): cv.i2c_address})
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = await binary_sensor.new_binary_sensor(config)
    await cg.register_component(var, config)

    cg.add(var.set_address(config[CONF_ADDRESS]))

    cg.add_library("adafruit/Adafruit BusIO", None)
    cg.add_library("adafruit/Adafruit seesaw Library", None)
