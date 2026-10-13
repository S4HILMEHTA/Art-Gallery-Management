#!/usr/bin/env python3
"""Automated test runner for the Art Gallery Management System.

Each test starts with an empty data folder, optionally runs set-up sessions
(which save their data on exit), then runs the final session and checks the
program output for the expected text.

Usage (from the project folder, after building the program):
    python3 tests/run_tests.py
"""
import json, os, re, shutil, subprocess, sys, tempfile

EXE = "./ArtGallery.exe" if os.name == "nt" else "./ArtGallery"

def artist(name, style):      return ["1", "1", name, style]
def artwork(t, aid, price):   return ["1", "2", t, str(aid), str(price)]
def customer(name, phone):    return ["1", "3", name, phone]
def exhibition(n, d, v):      return ["1", "4", n, d, v]
EXIT = ["9"]

VG = artist("Vincent van Gogh", "Post-Impressionism")
SN = artwork("Starry Night", 101, 250000)
RS = customer("Rahul Shah", "9876543210")
EX = exhibition("Spring Impressions 2026", "20/11/2026", "Ahmedabad Art Hall")

TESTS = [
 ("T01", "Add Artist (valid)", "Name: Vincent van Gogh, Style: Post-Impressionism",
  "Artist added with ID 101", [], VG, ["Artist added successfully with ID 101"]),
 ("T02", "Add Artist - validation", "Empty name, then valid name",
  "Error shown, re-prompt, then artist added", [], ["1", "1", "", "Claude Monet", "Impressionism"],
  ["Artist name cannot be empty", "Artist added successfully"]),
 ("T03", "Add Artwork - invalid artist", "Title: Starry Night, Artist ID: 999",
  "Exception: Artist ID 999 not found", [VG], ["1", "2", "Starry Night", "999"],
  ["Artist ID 999 not found"]),
 ("T04", "Add Artwork (valid)", "Starry Night, Artist 101, Rs. 250000",
  "Artwork added with ID 1001", [VG], SN, ["Artwork added successfully with ID 1001"]),
 ("T05", "Add Artwork - validation", "Price: -500, then 250000",
  "Negative price rejected, then accepted", [VG], ["1", "2", "Starry Night", "101", "-500", "250000"],
  ["price must be a positive number", "Artwork added successfully"]),
 ("T06", "Add Customer - validation", "Phone: 12345, then 9876543210",
  "Short phone rejected, then customer added", [], ["1", "3", "Rahul Shah", "12345", "9876543210"],
  ["exactly 10 digits", "Customer added successfully with ID 201"]),
 ("T07", "Display Artworks", "Menu 2 -> Artwork (2 artworks stored)",
  "Both artworks listed in a table", [VG, SN, artwork("Sunflowers", 101, 210000)], ["2", "2"],
  ["Starry Night", "Sunflowers", "ARTWORKS (2)"]),
 ("T08", "Search Artwork by keyword", "Menu 3 -> Artwork -> 'starry'",
  "Matching artwork shown (case-insensitive)", [VG, SN, artwork("Sunflowers", 101, 210000)],
  ["3", "2", "starry"], ["Starry Night", "1 record(s) found"]),
 ("T09", "Search - record not found", "Menu 3 -> Artwork -> '9999'",
  "Message: No record found", [VG, SN], ["3", "2", "9999"], ["No record found"]),
 ("T10", "Update Artwork price", "Artwork 1001, keep title, new price 300000",
  "Price updated to Rs. 300000.00", [VG, SN],
  ["4", "2", "1001", "", "300000", "", "2", "2"], ["Artwork updated", "Rs. 300000.00"]),
 ("T11", "Delete Artist who has artworks", "Delete Artist ID 101, confirm y",
  "Deletion blocked by business rule", [VG, SN], ["5", "1", "101", "y"],
  ["Cannot delete: this artist still has artworks"]),
 ("T12", "Sell Artwork (main transaction)", "Artwork 1001 sold to Customer 201",
  "Sale receipt; artwork status becomes Sold", [VG, SN, RS],
  ["6", "1", "1001", "201", "2", "2"],
  ["Sale completed successfully", "SALE RECEIPT", "Rs. 250000.00     Sold"]),
 ("T13", "Sell already-sold Artwork", "Sell Artwork 1001 again",
  "Exception: artwork is already sold", [VG, SN, RS, ["6", "1", "1001", "201"]],
  ["6", "1", "1001", "201"], ["is already sold"]),
 ("T14", "Organize Exhibition", "Add Artwork 1001 to Exhibition 301",
  "Artwork added; status becomes Exhibited", [VG, SN, EX], ["6", "3", "301", "1001"],
  ["added to exhibition", "Exhibited"]),
 ("T15", "File persistence (restart)", "Add data, exit, restart program, display artworks",
  "Old records loaded automatically", [VG, SN], ["2", "2"],
  ["Loaded: 1 artists, 1 artworks", "Starry Night"]),
]

def run(data_dir, lines):
    text = "\n".join(lines + EXIT) + "\n"
    p = subprocess.run([EXE, data_dir], input=text, capture_output=True, text=True, timeout=30)
    return p.stdout

def main():
    results, failed = [], 0
    for tid, op, inp, expected, setup, steps, checks in TESTS:
        d = tempfile.mkdtemp()
        try:
            for s in setup:
                run(d, s)
            out = run(d, steps)
        finally:
            shutil.rmtree(d, ignore_errors=True)
        low = out.lower()
        ok = all(c.lower() in low for c in checks)
        actual = []
        for c in checks:
            for line in out.splitlines():
                if c.lower() in line.lower():
                    actual.append(re.sub(r"\s+", " ", line).strip()); break
        results.append(dict(id=tid, operation=op, input=inp, expected=expected,
                            actual=" | ".join(dict.fromkeys(actual)),
                            status="PASS" if ok else "FAIL"))
        failed += 0 if ok else 1
        print(f"{tid}  {'PASS' if ok else 'FAIL'}  {op}")
    with open(os.path.join(os.path.dirname(os.path.abspath(__file__)), "results.json"), "w") as f:
        json.dump(results, f, indent=2)
    print(f"\n{len(TESTS) - failed}/{len(TESTS)} tests passed")
    sys.exit(1 if failed else 0)

if __name__ == "__main__":
    main()
