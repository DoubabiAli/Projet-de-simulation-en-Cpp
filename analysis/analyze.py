import sys
from pathlib import Path

import matplotlib
import pandas as pd

matplotlib.use("Agg")
import matplotlib.pyplot as plt

DATA_DIR = Path(sys.argv[1] if len(sys.argv) > 1 else "data/sample")
OUT = Path("docs/analysis")
OUT.mkdir(parents=True, exist_ok=True)

UNITS = {"Temperature": "°C", "Humidity": "%", "Sound": "dB"}
# Plage normale du simulateur (valeurs entre 0 et 100) : en dehors, la mesure est anormale
VALID_RANGE = {"Temperature": (0, 100), "Humidity": (0, 100), "Sound": (0, 100)}
WINDOW, Z_MAX = 20, 3.0  # fenêtre glissante et seuil du z-score


def load(name):
    """Lit <name>.csv (séparateur ';', colonnes Date;Heure;SensorId;SensorType;Valeur)."""
    df = pd.read_csv(DATA_DIR / f"{name}.csv", sep=";")
    df["ts"] = pd.to_datetime(df["Date"] + " " + df["Heure"], format="%Y-%m-%d %H:%M:%S")
    return df.sort_values("ts").reset_index(drop=True)


lines = ["# Rapport d'analyse des mesures simulées", ""]
all_anomalies = []

for name, unit in UNITS.items():
    df = load(name)
    v = pd.to_numeric(df["Valeur"], errors="coerce")
    d = v.describe()

    lo, hi = VALID_RANGE[name]
    out_of_range = (v < lo) | (v > hi)                        # hors plage
    mean = v.rolling(WINDOW, min_periods=5).mean().shift(1)
    std = v.rolling(WINDOW, min_periods=5).std().shift(1)
    spike = ((v - mean) / std).abs() > Z_MAX                  # pic statistique
    mask = (out_of_range | spike).fillna(False)

    anom = df[mask].assign(raison=["hors plage" if o else "pic" for o in out_of_range[mask]])
    all_anomalies.append(anom)

    lines += [
        f"## {name} ({unit})", "",
        f"- mesures : {len(df)}",
        f"- moyenne : {d['mean']:.2f}, écart-type : {d['std']:.2f}",
        f"- min : {d['min']:.2f}, médiane : {d['50%']:.2f}, max : {d['max']:.2f}",
        f"- anomalies : {int(mask.sum())} ({100 * mask.mean():.1f} %)", "",
        f"![{name}]({name}.png)", "",
    ]

    plt.figure(figsize=(9, 3))
    plt.plot(df["ts"], v, linewidth=0.8)
    plt.scatter(df["ts"][mask], v[mask], color="red", s=22, zorder=3, label="anomalie")
    plt.title(f"{name} : mesures et anomalies")
    plt.ylabel(unit)
    if mask.any():
        plt.legend()
    plt.tight_layout()
    plt.savefig(OUT / f"{name}.png", dpi=120)
    plt.close()

light = load("Light")
state = light["Valeur"].astype(str).str.lower().map({"true": 1, "false": 0})
lines += [
    "## Light (allumé / éteint)", "",
    f"- mesures : {len(light)}",
    f"- part du temps allumé : {100 * state.mean():.1f} %",
    f"- changements d'état : {int((state.diff().abs() == 1).sum())}", "",
]

pd.concat(all_anomalies).to_csv(OUT / "anomalies.csv", index=False, sep=";")
(OUT / "report.md").write_text("\n".join(lines), encoding="utf-8")
print(f"Rapport généré dans {OUT.as_posix()}/")