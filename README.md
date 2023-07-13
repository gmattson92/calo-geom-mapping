# calo-geom-mapping
Calorimeter tower geometry mapping for data reconstruction

The CaloGeomMapping class will add the calorimeter tower-to-position mapping to the node tree in the reconstructed data DSTs. It reads the geometry info from a file "CALO_TOWER_GEOMETRY" in the CDB.
The class is defined in `src`. `macros` has a Fun4All macro to run this module on an existing data file.

`calo_maps` contains two subdirectories:
 - `write_to_csv` contains a module and corresponding macro to write out the calo geometry info to text files "emcal.txt", "ihcal.txt" and "ohcal.txt"
 - `CDBTTree` contains a macro to read the above text files and create a CDBTTree object with this information, and writes it to `calo_geom_mapping.root`. This is the file which Chris needs to add to the CDB
