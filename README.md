# Multiresolution - Delete Lower

This Blender addon allows you to delete lower subdivision levels of a Multiresolution modifier and transfer vertex indices between objects.

## Description

This addon provides tools for managing multiresolution meshes in Blender. It allows you to delete lower subdivision levels and transfer vertex indices from one object to another to facilitate multiresolution reshaping.

## Features

- **Delete Lower Subdivision Levels**: Deletes lower subdivision levels of the active object's multiresolution modifier.
- **Transfer Vertex Index Order**: Transfers vertex index order from a source object to a target object, allowing for proper multiresolution reshaping.

## Installation

1. Download the `MultiResDeleteLower_dll.py` script.
2. Open Blender.
3. Go to `Edit > Preferences > Add-ons`.
4. Click `Install...` and select the downloaded script.
5. Enable the addon by checking the box next to `Multiresolution - Delete Lower`.

## Usage

### Delete Lower Subdivision Levels

1. Select the object with a multiresolution modifier.
2. Open the sidebar (press `N` if it's not visible).
3. Go to the `Tools` panel.
4. Click `Delete Lower` to delete lower subdivision levels of the selected object.

### Transfer Vertex Index Order

1. Select the source object and target object.
2. Make sure the source object is active.
3. Open the sidebar and go to the `Tools` panel.
4. Click `Transfer Vertex Order` to transfer vertex indices from the active object to the selected object.

## Detailed Code Description

### `MultiResDeleteLower.py`

- **Imports and Info Block**: Necessary imports and Blender addon metadata.
- **`index_transfer` Function**: 
  - Transfers vertex index order from the source object to the target object using KDTree for distance calculations.
  - Initializes and populates the KDTree with vertex UV coordinates and indices.
  - Finds and matches vertices in the target object based on the closest UV coordinates.
- **`apply_multires` Function**: Turns off UV and boundary smoothing, then applies the multiresolution modifier.
- **`del_lower` Function**: 
  - Duplicates objects and applies multiresolution modifications.
  - Uses `index_transfer` to match vertex indices between duplicates.
  - Applies reshaping and cleans up duplicates.
- **`DeleteLower` Operator**: Deletes lower subdivision levels.
- **`TransferVertexIndex` Operator**: Transfers vertex index order.
- **`OBJECT_PT_CustomPanel` Class**: Custom panel in the 3D view sidebar for easy access to operators.
- **Register and Unregister Functions**: Registers and unregisters the classes with Blender.

## Dependencies

- Blender 3.6.2 or later

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

Special thanks to the Blender community for their continuous support and contributions.

