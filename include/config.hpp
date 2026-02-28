#pragma once

#include <string>
#include <unordered_map>
#include <tomlplusplus/toml.hpp>

using namespace std;

namespace asim {

inline static toml::table current_config;

// update README if you add more
inline const unordered_map<string, string_view> configs = {
    // goob is just an empty config
    {"goob", ""sv},
    // Up to date as of 14.02.2026
    {"wizden", R"(
        [Tritium]
        FireEnergyReleased = 2840000
        BurnFuelRatio = 2

        # [Frezon]
        # ProductionTritRatio = 8
        # Unnerfed in https://github.com/space-wizards/space-station-14/pull/42400
    )"sv},
    // Up to date as of 14.02.2026
    {"frontier", R"(
        [Plasma]
        SuperSaturationThreshold = 30
        UpperTemperature = 700

        [Reactions]
        TritiumFireTemp = 700
    )"sv},
    // Identical to frontier but 10 liter tanks
    // Up to date as of 14.02.2026
    {"monolith", R"(
        [Plasma]
        SuperSaturationThreshold = 30
        UpperTemperature = 700

        [Reactions]
        TritiumFireTemp = 700

        [Tank]
        Volume = 10
    )"sv},
};

inline bool parse_config(string config) {
    try {
        current_config = toml::parse(configs.at(config));
        return true;
    } catch (...) { }
    try { // inshallah are you a file
        current_config = toml::parse_file(config);
        return true;
    } catch (...) { }
    return false;
}

// GOIDA INCOMING
inline float
    default_tol,
    heat_scale,
    R,
    one_atmosphere,
    TCMB,
    T0C,
    T20C,
    minimum_heat_capacity,
    fire_plasma_energy_released,
    super_saturation_threshold,
    super_saturation_ends,
    oxygen_burn_rate_base,
    plasma_minimum_burn_temperature,
    plasma_upper_temperature,
    plasma_oxygen_fullburn,
    plasma_burn_rate_delta,
    fire_hydrogen_energy_released,
    minimum_tritium_oxyburn_energy,
    tritium_burn_oxy_factor,
    tritium_burn_trit_factor,
    tritium_burn_fuel_ratio,
    frezon_cool_lower_temperature,
    frezon_cool_mid_temperature,
    frezon_cool_maximum_energy_modifier,
    frezon_nitrogen_cool_ratio,
    frezon_cool_energy_released,
    frezon_cool_rate_modifier,
    frezon_production_temp,
    frezon_production_max_efficiency_temperature,
    frezon_production_nitrogen_ratio,
    frezon_production_trit_ratio,
    frezon_production_conversion_rate,
    N2Odecomposition_rate,
    nitrium_decomposition_energy,
    reaction_min_gas,
    plasma_fire_temp,
    trit_fire_temp,
    frezon_cool_temp,
    n2o_decomp_temp,
    nitrium_decomp_temp,
    pressure_cap,
    required_transfer_volume,
    tank_volume,
    tank_leak_pressure,
    tank_rupture_pressure,
    tank_fragment_pressure,
    tank_fragment_scale,
    tickrate;

inline bool set_config(string config) {
  if (parse_config(config)) {
        // goobstation (non-reforged) defaults; up to date as of 14.02.2026
        // [Atmosim]
        default_tol = current_config["Atmosim"]["DefaultTolerance"].value_or(0.95f);

        // [Cvars]
        heat_scale = current_config["Cvars"]["HeatScale"].value_or(1.0 / 8.f); // inverted

        // [Atmospherics]
        R = current_config["Atmospherics"]["R"].value_or(8.314462618f);
        one_atmosphere = current_config["Atmospherics"]["OneAtmosphere"].value_or(101.325f);
        TCMB = current_config["Atmospherics"]["TCMB"].value_or(2.7f);
        T0C = current_config["Atmospherics"]["T0C"].value_or(273.15f);
        T20C = current_config["Atmospherics"]["T20C"].value_or(293.15f);
        minimum_heat_capacity = current_config["Atmospherics"]["MinimumHeatCapacity"].value_or(0.0003f);

        // [Plasma]
        fire_plasma_energy_released = current_config["Plasma"]["FireEnergyReleased"].value_or(160000.f) * heat_scale;
        super_saturation_threshold = current_config["Plasma"]["SuperSaturationThreshold"].value_or(96.f);
        super_saturation_ends = current_config["Plasma"]["SuperSaturationEnds"].value_or(super_saturation_threshold / 3.f);
        oxygen_burn_rate_base = current_config["Plasma"]["OxygenBurnRateBase"].value_or(1.4f);
        plasma_minimum_burn_temperature = current_config["Plasma"]["MinimumBurnTemperature"].value_or(100.f + T0C);
        plasma_upper_temperature = current_config["Plasma"]["UpperTemperature"].value_or(1370.f + T0C);
        plasma_oxygen_fullburn = current_config["Plasma"]["OxygenFullburn"].value_or(10.f);
        plasma_burn_rate_delta = current_config["Plasma"]["BurnRateDelta"].value_or(9.f);

        // [Tritium]
        fire_hydrogen_energy_released = current_config["Tritium"]["FireEnergyReleased"].value_or(284000.f) * heat_scale;
        minimum_tritium_oxyburn_energy = current_config["Tritium"]["MinimumOxyburnEnergy"].value_or(143000.f) * heat_scale;
        tritium_burn_oxy_factor = current_config["Tritium"]["BurnOxyFactor"].value_or(100.f);
        tritium_burn_trit_factor = current_config["Tritium"]["BurnTritFactor"].value_or(10.f);
        tritium_burn_fuel_ratio = current_config["Tritium"]["BurnFuelRatio"].value_or(0.f);

        // [Frezon]
        frezon_cool_lower_temperature = current_config["Frezon"]["CoolLowerTemperature"].value_or(23.15f);
        frezon_cool_mid_temperature = current_config["Frezon"]["CoolMidTemperature"].value_or(373.15f);
        frezon_cool_maximum_energy_modifier = current_config["Frezon"]["CoolMaximumEnergyModifier"].value_or(10.f);
        frezon_nitrogen_cool_ratio = current_config["Frezon"]["NitrogenCoolRatio"].value_or(5.f);
        frezon_cool_energy_released = current_config["Frezon"]["CoolEnergyReleased"].value_or(-600000.f) * heat_scale;
        frezon_cool_rate_modifier = current_config["Frezon"]["CoolRateModifier"].value_or(20.f);
        frezon_production_temp = current_config["Frezon"]["ProductionTemp"].value_or(73.15f);
        frezon_production_max_efficiency_temperature = current_config["Frezon"]["ProductionMaxEfficiencyTemperature"].value_or(73.15f);
        frezon_production_nitrogen_ratio = current_config["Frezon"]["ProductionNitrogenRatio"].value_or(10.f);
        frezon_production_trit_ratio = current_config["Frezon"]["ProductionTritRatio"].value_or(50.f);
        frezon_production_conversion_rate = current_config["Frezon"]["ProductionConversionRate"].value_or(50.f);

        // [N20]
        N2Odecomposition_rate = current_config["N20"]["DecompositionRate"].value_or(1.f / 2.f); // inverted

        // [Nitrium]
        nitrium_decomposition_energy = current_config["Nitrium"]["DecompositionEnergy"].value_or(30000.f);

        // [Reactions]
        reaction_min_gas = current_config["Reactions"]["ReactionMinGas"].value_or(0.01f);
        plasma_fire_temp = current_config["Reactions"]["PlasmaFireTemp"].value_or(373.149f);
        trit_fire_temp = current_config["Reactions"]["TritiumFireTemp"].value_or(373.149f);
        frezon_cool_temp = current_config["Reactions"]["FrezonCoolTemp"].value_or(23.15f);
        n2o_decomp_temp = current_config["Reactions"]["N2ODecomposionTemp"].value_or(850.f);
        nitrium_decomp_temp = current_config["Reactions"]["NitriumDecompositionTemp"].value_or(T0C + 70.f);

        // [Canister]
        pressure_cap = current_config["Canister"]["TransferPressureCap"].value_or(1013.25f);
        required_transfer_volume = current_config["Canister"]["RequiredTransferVolume"].value_or(1500.f + 200.f * 2); // canister + two pipes volume

        // [Tank]
        tank_volume = current_config["Tank"]["Volume"].value_or(5.f);
        tank_leak_pressure = current_config["Tank"]["LeakPressure"].value_or(30.f * one_atmosphere);
        tank_rupture_pressure = current_config["Tank"]["RupturePressure"].value_or(40.f * one_atmosphere);
        tank_fragment_pressure = current_config["Tank"]["FragmentPressure"].value_or(50.f * one_atmosphere);
        tank_fragment_scale = current_config["Tank"]["FragmentScale"].value_or(2.25f * one_atmosphere);

        // [Misc]
        tickrate = current_config["Misc"]["Tickrate"].value_or(0.5f);
        return true;
    }
    return false;
}

inline const size_t round_temp_dig = 2, round_pressure_dig = 1;

}
