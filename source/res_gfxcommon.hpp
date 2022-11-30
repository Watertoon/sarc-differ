 /*
 *  Copyright (C) W. Michael Knudson
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as 
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License along with this program; 
 *  if not, see <https://www.gnu.org/licenses/>.
 */
#pragma once

namespace dd::res {

    enum GfxGpuAccessFlags {
        GfxGpuAccessFlags_CpuNoAccess  = (1 << 0),
        GfxGpuAccessFlags_CpuUncached  = (1 << 1),
        GfxGpuAccessFlags_CpuCached    = (1 << 2),
        GfxGpuAccessFlags_GpuNoAccess  = (1 << 3),
        GfxGpuAccessFlags_GpuUncached  = (1 << 4),
        GfxGpuAccessFlags_GpuCached    = (1 << 5),
        GfxGpuAccessFlags_ShaderCode   = (1 << 6),
        GfxGpuAccessFlags_Compressible = (1 << 7),
        GfxGpuAccessFlags_Physical     = (1 << 8),
        GfxGpuAccessFlags_Virtual      = (1 << 9),
    };

    struct ResGfxMemoryPoolInfo {
        u32   gpu_access_flags;
        u32   size;
        void *storage;
    };
    static_assert(sizeof(ResGfxMemoryPoolInfo) == 0x10);

    struct ResGfxBufferInfo {
        u32 size;
        u32 gpu_access_flags;
        u32 reserve0;
        u32 reserve1;
    };
    static_assert(sizeof(ResGfxBufferInfo) == 0x10);

    enum class GfxChannelSource : u8 {
        Zero  = 0x0,
        One   = 0x1,
        Red   = 0x2,
        Green = 0x3,
        Blue  = 0x4,
        Alpha = 0x5
    };

    enum class GfxChannelFormat : u8 {
        None         = 0x0,
        R8           = 0x2,
        R4G4B4A4     = 0x3,
        R5G5B5A1     = 0x5,
        A1B5G5R5     = 0x6,
        R5G6B5       = 0x7,
        B5G6R5       = 0x8,
        R8G8         = 0x9,
        R16          = 0xa,
        R8G8B8A8     = 0xb,
        B8G8R8A8     = 0xc,
        R9G9B9E5F    = 0xd,
        R10G10B10A2  = 0xe,
        R11G11B10F   = 0xf,
        R16G16       = 0x12,
        D24S8        = 0x13,
        R32          = 0x14,
        R16G16B16A16 = 0x15,
        D32FS8       = 0x16,
        R32G32       = 0x17,
        R32G32B32    = 0x18,
        R32G32B32A32 = 0x19,
        BC1          = 0x1a,
        BC3          = 0x1b,
        BC5          = 0x1c,
        BC4          = 0x1d,
        //BC5          = 0x1e,
        BC6H         = 0x1f,
        BC7U         = 0x20,
        ASTC_4x4     = 0x2d,
        ASTC_5x4     = 0x2e,
        ASTC_5x5     = 0x2f,
        ASTC_6x5     = 0x30,
        ASTC_6x6     = 0x31,
        ASTC_8x5     = 0x32,
        ASTC_8x6     = 0x33,
        ASTC_8x8     = 0x34,
        ASTC_10x5    = 0x35,
        ASTC_10x6    = 0x36,
        ASTC_10x8    = 0x37,
        ASTC_10x10   = 0x38,
        ASTC_12x10   = 0x39,
        ASTC_12x12   = 0x3a,
        B5G5R5A1     = 0x3b,
    };

    static constexpr u32 VariableBlockWidthTable[] = {
        0x10, 0x08, 0x10, 0x08,
        0x08, 0x04, 0x04, 0x05,
        0x05, 0x06, 0x06, 0x08,
        0x08, 0x08, 0x0a, 0x0a,
        0x0a, 0x0a, 0x0c, 0x0c,
    };
    static constexpr u32 VariableBlockHeightTable[] = {
        0x08, 0x08, 0x08, 0x08,
        0x04, 0x04, 0x04, 0x04,
        0x05, 0x05, 0x06, 0x05,
        0x06, 0x08, 0x05, 0x06,
        0x08, 0x0a, 0x0a, 0x0c,
    };
    
    static constexpr u32 PackagedTextureTexelSizeTable[] = {
        0x08, 0x10, 0x10, 0x08,
        0x10, 0x10, 0x10, 0x08,
        0x10, 0x08, 0x08, 0x08,
        0x10, 0x20, 0x20, 0x20,
        0x20, 0x08, 0x08, 0x10,
        0x10, 0x10, 0x10, 0x10,
        0x10, 0x10, 0x10, 0x10,
        0x10, 0x10, 0x10, 0x10,
        0x10, 0x02
    };

    constexpr ALWAYS_INLINE u32 CalculateGfxImageSize(GfxChannelFormat channel_format, u32 width, u32 height, u32 depth) {
        u32 format = static_cast<u32>(channel_format);

        /* Adjust width and height for compressed formats */
        u32 package_format = format - 0x1a;
        if (package_format < 0x21) {
            u32 variable_format = format - 0x27;
            u32 block_width     = 4;
            u32 block_height    = 4;
            if (variable_format < 0x14) {
                block_width  = VariableBlockWidthTable[variable_format];
                block_height = VariableBlockHeightTable[variable_format];
            }

            width  = (block_width != 0)  ? (width  + block_width  - 1) / block_width  : 0;
            height = (block_height != 0) ? (height + block_height - 1) / block_height : 0;
        }

        /* Find texel size */
        u32 texel_size = 0;
        if (package_format < 0x22) {
            texel_size = PackagedTextureTexelSizeTable[package_format];
        } else if (format < 0x3) {
            texel_size = 1;
        } else if (format < 0xb) {
            texel_size = 2;
        } else if (format < 0x15) {
            texel_size = 4;
        } else if (format < 0x18) {
            texel_size = 8;
        } else {
            texel_size = 12;
            if (format != 0x18) {
                texel_size = 16;
            }
        }

        /* Calculate */
        return texel_size * width * height * depth;
    }

    enum class GfxTypeFormat : u8 {
        Unorm = 1,
        Snorm = 2,
        UInt  = 3,
        SInt  = 4,
        Float = 5,
        SRGB  = 6,
        Depth = 7, /* (Unorm) */
        UFloat = 0xa
    };

    #define GFX_MAKE_IMAGE_FORMAT(channel_format, type_format) \
        (((static_cast<u32>(channel_format) & 0xFF) << 8) | (static_cast<u32>(type_format) & 0xFF))

    enum class GfxImageFormat : u32 {
        //R8_Unorm              = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8,           GfxTypeFormat::Unorm),
        //R8_Snorm              = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8,           GfxTypeFormat::Snorm),
        //R8_UInt               = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8,           GfxTypeFormat::UInt),
        //R8_SInt               = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8,           GfxTypeFormat::SInt),
        //R4G4B4A4_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R4G4B4A4,     GfxTypeFormat::Unorm),
        //R5G5B5A1_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R5G5B5A1,     GfxTypeFormat::Unorm),
        //A1B5G5R5_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::A1B5G5R5,     GfxTypeFormat::Unorm),
        //R5G6B5_Unorm          = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R5G6B5,       GfxTypeFormat::Unorm),
        //B5G6R5_Unorm          = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::B5G6R5,       GfxTypeFormat::Unorm),
        //R8G8_Unorm            = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8,         GfxTypeFormat::Unorm),
        //R8G8_Snorm            = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8,         GfxTypeFormat::Snorm),
        //R8G8_UInt             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8,         GfxTypeFormat::UInt),
        //R8G8_SInt             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8,         GfxTypeFormat::SInt),
        //R16_Unorm             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16,          GfxTypeFormat::Unorm),
        //R16_Snorm             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16,          GfxTypeFormat::Snorm),
        //R16_UInt              = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16,          GfxTypeFormat::UInt),
        //R16_SInt              = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16,          GfxTypeFormat::SInt),
        //R16_Float             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16,          GfxTypeFormat::Float),
        //Z16_Depth             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16,          GfxTypeFormat::Depth),
        //R8G8B8A8_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8B8A8,     GfxTypeFormat::Unorm),
        //R8G8B8A8_Snorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8B8A8,     GfxTypeFormat::Snorm),
        //R8G8B8A8_UInt         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8B8A8,     GfxTypeFormat::UInt),
        //R8G8B8A8_SInt         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8B8A8,     GfxTypeFormat::SInt),
        //R8G8B8A8_SRGB         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8B8A8,     GfxTypeFormat::SRGB),
        //B8G8R8A8_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::B8G8R8A8,     GfxTypeFormat::Unorm),
        //B8G8R8A8_SRGB         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::B8G8R8A8,     GfxTypeFormat::SRGB),
        //R9G9B9E5F_SharedFloat = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R9G9B9E5F,    GfxTypeFormat::Float),
        //R10G10B10A2_Unorm     = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R10G10B10A2,  GfxTypeFormat::Unorm),
        //R10G10B10A2_UInt      = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R10G10B10A2,  GfxTypeFormat::UInt),
        //R11G11B10F_Float      = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R11G11B10F,   GfxTypeFormat::Float),
        //R16G16_Unorm          = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16,       GfxTypeFormat::Unorm),
        //R16G16_Snorm          = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16,       GfxTypeFormat::Snorm),
        //R16G16_UInt           = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16,       GfxTypeFormat::UInt),
        //R16G16_SInt           = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16,       GfxTypeFormat::SInt),
        //R16G16_Float          = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16,       GfxTypeFormat::Float),
        //D24S8_Depth           = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::D24S8,        GfxTypeFormat::Depth),
        //R32_UInt              = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32,          GfxTypeFormat::UInt),
        //R32_SInt              = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32,          GfxTypeFormat::SInt),
        //R32_Float             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32,          GfxTypeFormat::Float),
        //D32F_Depth            = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32,          GfxTypeFormat::Depth),
        //R16G16B16A16_Unorm    = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16B16A16, GfxTypeFormat::Unorm),
        //R16G16B16A16_Snorm    = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16B16A16, GfxTypeFormat::Snorm),
        //R16G16B16A16_UInt     = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16B16A16, GfxTypeFormat::UInt),
        //R16G16B16A16_SInt     = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16B16A16, GfxTypeFormat::SInt),
        //R16G16B16A16_Float    = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16B16A16, GfxTypeFormat::Float),
        //D32FS8_Depth          = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::D32FS8,       GfxTypeFormat::Depth),
        //R32G32_UInt           = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32,       GfxTypeFormat::UInt),
        //R32G32_SInt           = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32,       GfxTypeFormat::SInt),
        //R32G32_Float          = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32,       GfxTypeFormat::Float),
        //R32G32B32_UInt        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32B32,    GfxTypeFormat::UInt),
        //R32G32B32_SInt        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32B32,    GfxTypeFormat::SInt),
        //R32G32B32_Float       = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32B32,    GfxTypeFormat::Float),
        //R32G32B32A32_UInt     = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32B32A32, GfxTypeFormat::UInt),
        //R32G32B32A32_SInt     = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32B32A32, GfxTypeFormat::SInt),
        //R32G32B32A32_Float    = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32B32A32, GfxTypeFormat::Float),
        //BC1_Unorm             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC1,          GfxTypeFormat::Unorm),
        //BC1_SRGB              = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC1,          GfxTypeFormat::SRGB),
        //BC2_Unorm             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC2,          GfxTypeFormat::Unorm),
        //BC2_SRGB              = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC2,          GfxTypeFormat::SRGB),
        //BC3_Unorm             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC3,          GfxTypeFormat::Unorm),
        //BC3_SRGB              = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC3,          GfxTypeFormat::SRGB),
        //BC4_Unorm             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC4,          GfxTypeFormat::Unorm),
        //BC4_Snorm             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC4,          GfxTypeFormat::Snorm),
        //BC5_Unorm             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC5,          GfxTypeFormat::Unorm),
        //BC5_Snorm             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC5,          GfxTypeFormat::Unorm),
        //BC6H_SF16             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC6,          GfxTypeFormat::Float),
        //BC6H_UF16             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC6,          GfxTypeFormat::UFloat),
        //BC7U_Unorm            = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC7,          GfxTypeFormat::Unorm),
        //BC7U_SRGB             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC7,          GfxTypeFormat::SRGB),
        //ASTC_4X4_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_4x4,     GfxTypeFormat::Unorm),
        //ASTC_4X4_SRGB         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_4x4,     GfxTypeFormat::SRGB),
        //ASTC_5X4_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_5x4,     GfxTypeFormat::Unorm),
        //ASTC_5X4_SRGB         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_5x4,     GfxTypeFormat::SRGB),
        //ASTC_5X5_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_5x5,     GfxTypeFormat::Unorm),
        //ASTC_5X5_SRGB         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_5x5,     GfxTypeFormat::SRGB),
        //ASTC_6X5_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_6x5,     GfxTypeFormat::Unorm),
        //ASTC_6X5_SRGB         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_6x5,     GfxTypeFormat::SRGB),
        //ASTC_6X6_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_6x6,     GfxTypeFormat::Unorm),
        //ASTC_6X6_SRGB         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_6x6,     GfxTypeFormat::SRGB),
        //ASTC_8X5_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_8x5,     GfxTypeFormat::Unorm),
        //ASTC_8X5_SRGB         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_8x5,     GfxTypeFormat::SRGB),
        //ASTC_8X6_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_8x6,     GfxTypeFormat::Unorm),
        //ASTC_8X6_SRGB         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_8x6,     GfxTypeFormat::SRGB),
        //ASTC_8X8_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_8x8,     GfxTypeFormat::Unorm),
        //ASTC_8X8_SRGB         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_8x8,     GfxTypeFormat::SRGB),
        //ASTC_10X5_Unorm       = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_10x5,    GfxTypeFormat::Unorm),
        //ASTC_10X5_SRGB        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_10x5,    GfxTypeFormat::SRGB),
        //ASTC_10X6_Unorm       = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_10x6,    GfxTypeFormat::Unorm),
        //ASTC_10X6_SRGB        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_10x6,    GfxTypeFormat::SRGB),
        //ASTC_10X8_Unorm       = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_10x8,    GfxTypeFormat::Unorm),
        //ASTC_10X8_SRGB        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_10x8,    GfxTypeFormat::SRGB),
        //ASTC_10X10_Unorm      = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_10x10,   GfxTypeFormat::Unorm),
        //ASTC_10X10_SRGB       = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_10x10,   GfxTypeFormat::SRGB),
        //ASTC_12X10_Unorm      = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_12x10,   GfxTypeFormat::Unorm),
        //ASTC_12X10_SRGB       = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_12x10,   GfxTypeFormat::SRGB),
        //ASTC_12X12_Unorm      = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_12x12,   GfxTypeFormat::Unorm),
        //ASTC_12X12_SRGB       = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_12x12,   GfxTypeFormat::SRGB),
        //B5G5R5A1_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::B5G5R5A1,     GfxTypeFormat::Unorm)
    };
    
    enum class GfxAttributeFormat : u32 {
        
    };

    enum class GfxImageStorageDimension : u8 { 
        Type1D = 1,
        Type2D = 2,
        Type3D = 3
    };

    enum class GfxImageDimension : u8 {
        Type1D                      = 0,
        Type2D                      = 1,
        Type3D                      = 2,
        TypeCube                    = 3,
        TypeCube1DArray             = 4,
        TypeCube2DArray             = 5,
        TypeCube2DMultisample       = 6,
        TypeCube2DMultisampleArray  = 7,
        TypeCubeArray               = 8
    };

    struct ResGfxTextureInfo {
        u8  is_packaged         : 1;
        u8  is_sparse_binding   : 1;
        u8  is_sparse_residency : 1;
        u8  is_res_texture      : 1;
        u8  reserve0            : 4;
        u8  storage_dimension;
        u16 tile_mode;
        u16 swizzle;
        u16 mip_levels;
        u16 sample_count;
        u16 reserve1;
        u32 image_format;
        u32 gpu_access_flag;
        u32 width;
        u32 height;
        u32 depth;
        u32 texture_dimension;
        u32 packaged_texture_layout;
    };
    static_assert(sizeof(ResGfxTextureInfo) == 0x28);

    struct ResGfxSamplerInfo {
        u8    wrap_mode_u;
        u8    wrap_mode_v;
        u8    wrap_mode_w;
        u8    compare_op;
        u8    border_color;
        u8    max_anisotropy;
        u8    enable_compare_op;
        u8    reduction_filter;
        float lod_clamp_min;
        float lod_clamp_max;
        float lod_bias;
        u32   reserve0;
        u32   reserve1;
        u32   reserve2;
    };
    static_assert(sizeof(ResGfxSamplerInfo) == 0x20);

    enum GfxPrimitiveTopology {
        GfxPrimitiveTopology_Points,
        GfxPrimitiveTopology_Lines,
        GfxPrimitiveTopology_LineStrip,
        GfxPrimitiveTopology_Triangles,
        GfxPrimitiveTopology_TrianglesStrip,
        GfxPrimitiveTopology_LinesAdjacency,
        GfxPrimitiveTopology_LineStripAdjacency,
        GfxPrimitiveTopology_TrianglesAdjacency,
        GfxPrimitiveTopology_TriangleStripAdjacency,
        GfxPrimitiveTopology_Patches,
    };

    enum GfxPolygonMode {
        GfxPolygonMode_Point,
        GfxPolygonMode_Fill,
        GfxPolygonMode_Line
    };

    struct ResGfxEmbedFile {
        void *file_offset;
        u32   file_size;
        u32   reserve0;
    };
    static_assert(sizeof(ResGfxEmbedFile) == 0x10);

    enum GfxUserDataType : u8 {
        GfxUserDataType_S64 = 1
    };

    struct ResGfxUserData {
        const char *user_data_name;
        void       *user_data;
        s32         data_entries;
        u8          data_type;
        u8          reserve0[43];
    };
    static_assert(sizeof(ResGfxUserData) == 0x40);
}
