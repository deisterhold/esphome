import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import (
    CONF_KEY,
    CONF_ID,
)
from . import neokey_ns, NeoKeyComponent

CODEOWNERS = ["@deisterhold"]

DEPENDENCIES = ["neokey"]

CONF_NEOKEY_ID = "neokey_id"

NeoKeyBinarySensor = neokey_ns.class_("NeoKeyBinarySensor", binary_sensor.BinarySensor)

CONFIG_SCHEMA = binary_sensor.binary_sensor_schema(NeoKeyBinarySensor).extend(
    {
        cv.GenerateID(CONF_NEOKEY_ID): cv.use_id(NeoKeyComponent),
        cv.Required(CONF_KEY): cv.int_range(min=0, max=3),
    }
)


async def to_code(config):
    neokey = await cg.get_variable(config[CONF_NEOKEY_ID])
    var = cg.new_Pvariable(
        config[CONF_ID],
        config[CONF_KEY],
    )
    await binary_sensor.register_binary_sensor(var, config)
    cg.add(neokey.register_key(var))
