cmake \
-G "Ninja" \
-DCMAKE_TOOLCHAIN_FILE="/opt/azurespheresdk/CMakeFiles/AzureSphereToolchain.cmake" \
-DAZURE_SPHERE_TARGET_API_SET="4" \
--no-warn-unused-cli \
-DCMAKE_MAKE_PROGRAM="ninja" \
..
