import XCTest
import SwiftUI
@testable import ColorFillingApp

class FiltersAndEffectsTests: XCTestCase {
    var filterManager: FilterManager!
    
    override func setUp() {
        super.setUp()
        filterManager = FilterManager()
    }
    
    override func tearDown() {
        filterManager = nil
        super.tearDown()
    }
    
    // MARK: - Filter Manager Initialization Tests
    
    func testFilterManagerInitialization() {
        XCTAssertEqual(filterManager.selectedFilter, .none, "Default filter should be none")
        XCTAssertEqual(filterManager.filterIntensity, 1.0, "Default filter intensity should be 1.0")
        XCTAssertFalse(filterManager.isPreviewMode, "Default preview mode should be false")
        XCTAssertEqual(filterManager.appliedFilters.count, 0, "Should have no applied filters initially")
    }
    
    // MARK: - Color Filters Tests
    
    func testBrightnessFilter() {
        filterManager.selectedFilter = .brightness
        filterManager.filterIntensity = 0.5
        
        XCTAssertEqual(filterManager.selectedFilter, .brightness, "Brightness filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.5, "Filter intensity should be set")
    }
    
    func testContrastFilter() {
        filterManager.selectedFilter = .contrast
        filterManager.filterIntensity = 0.8
        
        XCTAssertEqual(filterManager.selectedFilter, .contrast, "Contrast filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.8, "Filter intensity should be set")
    }
    
    func testSaturationFilter() {
        filterManager.selectedFilter = .saturation
        filterManager.filterIntensity = 1.2
        
        XCTAssertEqual(filterManager.selectedFilter, .saturation, "Saturation filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 1.2, "Filter intensity should be set")
    }
    
    func testHueFilter() {
        filterManager.selectedFilter = .hue
        filterManager.filterIntensity = 0.3
        
        XCTAssertEqual(filterManager.selectedFilter, .hue, "Hue filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.3, "Filter intensity should be set")
    }
    
    func testVibranceFilter() {
        filterManager.selectedFilter = .vibrance
        filterManager.filterIntensity = 0.7
        
        XCTAssertEqual(filterManager.selectedFilter, .vibrance, "Vibrance filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.7, "Filter intensity should be set")
    }
    
    func testColorBalanceFilter() {
        filterManager.selectedFilter = .colorBalance
        filterManager.filterIntensity = 0.6
        
        XCTAssertEqual(filterManager.selectedFilter, .colorBalance, "Color balance filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.6, "Filter intensity should be set")
    }
    
    func testLevelsFilter() {
        filterManager.selectedFilter = .levels
        filterManager.filterIntensity = 0.9
        
        XCTAssertEqual(filterManager.selectedFilter, .levels, "Levels filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.9, "Filter intensity should be set")
    }
    
    func testCurvesFilter() {
        filterManager.selectedFilter = .curves
        filterManager.filterIntensity = 0.4
        
        XCTAssertEqual(filterManager.selectedFilter, .curves, "Curves filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.4, "Filter intensity should be set")
    }
    
    func testColorizeFilter() {
        filterManager.selectedFilter = .colorize
        filterManager.filterIntensity = 0.8
        
        XCTAssertEqual(filterManager.selectedFilter, .colorize, "Colorize filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.8, "Filter intensity should be set")
    }
    
    func testSepiaFilter() {
        filterManager.selectedFilter = .sepia
        filterManager.filterIntensity = 1.0
        
        XCTAssertEqual(filterManager.selectedFilter, .sepia, "Sepia filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 1.0, "Filter intensity should be set")
    }
    
    func testBlackAndWhiteFilter() {
        filterManager.selectedFilter = .blackAndWhite
        filterManager.filterIntensity = 1.0
        
        XCTAssertEqual(filterManager.selectedFilter, .blackAndWhite, "Black and white filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 1.0, "Filter intensity should be set")
    }
    
    func testInvertFilter() {
        filterManager.selectedFilter = .invert
        filterManager.filterIntensity = 1.0
        
        XCTAssertEqual(filterManager.selectedFilter, .invert, "Invert filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 1.0, "Filter intensity should be set")
    }
    
    // MARK: - Blur Filters Tests
    
    func testGaussianBlurFilter() {
        filterManager.selectedFilter = .gaussianBlur
        filterManager.filterIntensity = 0.5
        
        XCTAssertEqual(filterManager.selectedFilter, .gaussianBlur, "Gaussian blur filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.5, "Filter intensity should be set")
    }
    
    func testMotionBlurFilter() {
        filterManager.selectedFilter = .motionBlur
        filterManager.filterIntensity = 0.7
        
        XCTAssertEqual(filterManager.selectedFilter, .motionBlur, "Motion blur filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.7, "Filter intensity should be set")
    }
    
    func testRadialBlurFilter() {
        filterManager.selectedFilter = .radialBlur
        filterManager.filterIntensity = 0.6
        
        XCTAssertEqual(filterManager.selectedFilter, .radialBlur, "Radial blur filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.6, "Filter intensity should be set")
    }
    
    func testZoomBlurFilter() {
        filterManager.selectedFilter = .zoomBlur
        filterManager.filterIntensity = 0.8
        
        XCTAssertEqual(filterManager.selectedFilter, .zoomBlur, "Zoom blur filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.8, "Filter intensity should be set")
    }
    
    func testBoxBlurFilter() {
        filterManager.selectedFilter = .boxBlur
        filterManager.filterIntensity = 0.4
        
        XCTAssertEqual(filterManager.selectedFilter, .boxBlur, "Box blur filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.4, "Filter intensity should be set")
    }
    
    // MARK: - Distortion Filters Tests
    
    func testBulgeFilter() {
        filterManager.selectedFilter = .bulge
        filterManager.filterIntensity = 0.5
        
        XCTAssertEqual(filterManager.selectedFilter, .bulge, "Bulge filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.5, "Filter intensity should be set")
    }
    
    func testPinchFilter() {
        filterManager.selectedFilter = .pinch
        filterManager.filterIntensity = 0.7
        
        XCTAssertEqual(filterManager.selectedFilter, .pinch, "Pinch filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.7, "Filter intensity should be set")
    }
    
    func testTwirlFilter() {
        filterManager.selectedFilter = .twirl
        filterManager.filterIntensity = 0.6
        
        XCTAssertEqual(filterManager.selectedFilter, .twirl, "Twirl filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.6, "Filter intensity should be set")
    }
    
    func testWaveFilter() {
        filterManager.selectedFilter = .wave
        filterManager.filterIntensity = 0.8
        
        XCTAssertEqual(filterManager.selectedFilter, .wave, "Wave filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.8, "Filter intensity should be set")
    }
    
    func testRippleFilter() {
        filterManager.selectedFilter = .ripple
        filterManager.filterIntensity = 0.4
        
        XCTAssertEqual(filterManager.selectedFilter, .ripple, "Ripple filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.4, "Filter intensity should be set")
    }
    
    func testGlassFilter() {
        filterManager.selectedFilter = .glass
        filterManager.filterIntensity = 0.9
        
        XCTAssertEqual(filterManager.selectedFilter, .glass, "Glass filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.9, "Filter intensity should be set")
    }
    
    // MARK: - Stylize Filters Tests
    
    func testEmbossFilter() {
        filterManager.selectedFilter = .emboss
        filterManager.filterIntensity = 0.5
        
        XCTAssertEqual(filterManager.selectedFilter, .emboss, "Emboss filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.5, "Filter intensity should be set")
    }
    
    func testReliefFilter() {
        filterManager.selectedFilter = .relief
        filterManager.filterIntensity = 0.7
        
        XCTAssertEqual(filterManager.selectedFilter, .relief, "Relief filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.7, "Filter intensity should be set")
    }
    
    func testEdgeWorkFilter() {
        filterManager.selectedFilter = .edgeWork
        filterManager.filterIntensity = 0.6
        
        XCTAssertEqual(filterManager.selectedFilter, .edgeWork, "Edge work filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.6, "Filter intensity should be set")
    }
    
    func testCrystallizeFilter() {
        filterManager.selectedFilter = .crystallize
        filterManager.filterIntensity = 0.8
        
        XCTAssertEqual(filterManager.selectedFilter, .crystallize, "Crystallize filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.8, "Filter intensity should be set")
    }
    
    func testPointillizeFilter() {
        filterManager.selectedFilter = .pointillize
        filterManager.filterIntensity = 0.4
        
        XCTAssertEqual(filterManager.selectedFilter, .pointillize, "Pointillize filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.4, "Filter intensity should be set")
    }
    
    func testPosterizeFilter() {
        filterManager.selectedFilter = .posterize
        filterManager.filterIntensity = 0.9
        
        XCTAssertEqual(filterManager.selectedFilter, .posterize, "Posterize filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.9, "Filter intensity should be set")
    }
    
    // MARK: - Sharpen Filters Tests
    
    func testSharpenFilter() {
        filterManager.selectedFilter = .sharpen
        filterManager.filterIntensity = 0.5
        
        XCTAssertEqual(filterManager.selectedFilter, .sharpen, "Sharpen filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.5, "Filter intensity should be set")
    }
    
    func testUnsharpMaskFilter() {
        filterManager.selectedFilter = .unsharpMask
        filterManager.filterIntensity = 0.7
        
        XCTAssertEqual(filterManager.selectedFilter, .unsharpMask, "Unsharp mask filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.7, "Filter intensity should be set")
    }
    
    func testEdgeEnhanceFilter() {
        filterManager.selectedFilter = .edgeEnhance
        filterManager.filterIntensity = 0.6
        
        XCTAssertEqual(filterManager.selectedFilter, .edgeEnhance, "Edge enhance filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.6, "Filter intensity should be set")
    }
    
    // MARK: - Noise Filters Tests
    
    func testNoiseReductionFilter() {
        filterManager.selectedFilter = .noiseReduction
        filterManager.filterIntensity = 0.5
        
        XCTAssertEqual(filterManager.selectedFilter, .noiseReduction, "Noise reduction filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.5, "Filter intensity should be set")
    }
    
    func testAddNoiseFilter() {
        filterManager.selectedFilter = .addNoise
        filterManager.filterIntensity = 0.7
        
        XCTAssertEqual(filterManager.selectedFilter, .addNoise, "Add noise filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.7, "Filter intensity should be set")
    }
    
    func testFilmGrainFilter() {
        filterManager.selectedFilter = .filmGrain
        filterManager.filterIntensity = 0.6
        
        XCTAssertEqual(filterManager.selectedFilter, .filmGrain, "Film grain filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.6, "Filter intensity should be set")
    }
    
    // MARK: - Artistic Filters Tests
    
    func testOilPaintingFilter() {
        filterManager.selectedFilter = .oilPainting
        filterManager.filterIntensity = 0.8
        
        XCTAssertEqual(filterManager.selectedFilter, .oilPainting, "Oil painting filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.8, "Filter intensity should be set")
    }
    
    func testWatercolorFilter() {
        filterManager.selectedFilter = .watercolor
        filterManager.filterIntensity = 0.7
        
        XCTAssertEqual(filterManager.selectedFilter, .watercolor, "Watercolor filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.7, "Filter intensity should be set")
    }
    
    func testSketchFilter() {
        filterManager.selectedFilter = .sketch
        filterManager.filterIntensity = 0.6
        
        XCTAssertEqual(filterManager.selectedFilter, .sketch, "Sketch filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.6, "Filter intensity should be set")
    }
    
    func testCartoonFilter() {
        filterManager.selectedFilter = .cartoon
        filterManager.filterIntensity = 0.9
        
        XCTAssertEqual(filterManager.selectedFilter, .cartoon, "Cartoon filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 0.9, "Filter intensity should be set")
    }
    
    func testVintageFilter() {
        filterManager.selectedFilter = .vintage
        filterManager.filterIntensity = 1.0
        
        XCTAssertEqual(filterManager.selectedFilter, .vintage, "Vintage filter should be selected")
        XCTAssertEqual(filterManager.filterIntensity, 1.0, "Filter intensity should be set")
    }
    
    // MARK: - Filter Intensity Tests
    
    func testFilterIntensityBounds() {
        // Test negative intensity
        filterManager.filterIntensity = -0.5
        XCTAssertGreaterThanOrEqual(filterManager.filterIntensity, 0.0, "Filter intensity should not be negative")
        
        // Test intensity greater than 1.0
        filterManager.filterIntensity = 2.0
        XCTAssertLessThanOrEqual(filterManager.filterIntensity, 1.0, "Filter intensity should not exceed 1.0")
        
        // Test valid intensity
        filterManager.filterIntensity = 0.5
        XCTAssertEqual(filterManager.filterIntensity, 0.5, "Valid filter intensity should be preserved")
    }
    
    func testFilterIntensityWithExtremeValues() {
        // Test very small intensity
        filterManager.filterIntensity = 0.001
        XCTAssertGreaterThan(filterManager.filterIntensity, 0.0, "Very small intensity should be preserved")
        
        // Test very large intensity
        filterManager.filterIntensity = 1000.0
        XCTAssertLessThanOrEqual(filterManager.filterIntensity, 1.0, "Very large intensity should be clamped")
    }
    
    // MARK: - Filter Application Tests
    
    func testApplyFilter() {
        filterManager.selectedFilter = .brightness
        filterManager.filterIntensity = 0.5
        
        let layer = createTestLayer()
        filterManager.applyFilter(to: layer)
        
        XCTAssertEqual(filterManager.appliedFilters.count, 1, "Should have one applied filter")
        XCTAssertEqual(filterManager.appliedFilters.first?.filter, .brightness, "Applied filter should be brightness")
        XCTAssertEqual(filterManager.appliedFilters.first?.intensity, 0.5, "Applied filter should have correct intensity")
    }
    
    func testApplyMultipleFilters() {
        let layer = createTestLayer()
        
        // Apply first filter
        filterManager.selectedFilter = .brightness
        filterManager.filterIntensity = 0.5
        filterManager.applyFilter(to: layer)
        
        // Apply second filter
        filterManager.selectedFilter = .contrast
        filterManager.filterIntensity = 0.7
        filterManager.applyFilter(to: layer)
        
        XCTAssertEqual(filterManager.appliedFilters.count, 2, "Should have two applied filters")
        XCTAssertEqual(filterManager.appliedFilters[0].filter, .brightness, "First filter should be brightness")
        XCTAssertEqual(filterManager.appliedFilters[1].filter, .contrast, "Second filter should be contrast")
    }
    
    func testApplyFilterWithZeroIntensity() {
        filterManager.selectedFilter = .brightness
        filterManager.filterIntensity = 0.0
        
        let layer = createTestLayer()
        filterManager.applyFilter(to: layer)
        
        XCTAssertEqual(filterManager.appliedFilters.count, 0, "Filter with zero intensity should not be applied")
    }
    
    func testApplyFilterWithMaximumIntensity() {
        filterManager.selectedFilter = .brightness
        filterManager.filterIntensity = 1.0
        
        let layer = createTestLayer()
        filterManager.applyFilter(to: layer)
        
        XCTAssertEqual(filterManager.appliedFilters.count, 1, "Filter with maximum intensity should be applied")
        XCTAssertEqual(filterManager.appliedFilters.first?.intensity, 1.0, "Applied filter should have maximum intensity")
    }
    
    // MARK: - Filter Removal Tests
    
    func testRemoveFilter() {
        let layer = createTestLayer()
        
        // Apply filter
        filterManager.selectedFilter = .brightness
        filterManager.filterIntensity = 0.5
        filterManager.applyFilter(to: layer)
        
        let appliedFilter = filterManager.appliedFilters.first!
        
        // Remove filter
        filterManager.removeFilter(appliedFilter)
        
        XCTAssertEqual(filterManager.appliedFilters.count, 0, "Should have no applied filters after removal")
    }
    
    func testRemoveNonExistentFilter() {
        let layer = createTestLayer()
        
        // Apply filter
        filterManager.selectedFilter = .brightness
        filterManager.filterIntensity = 0.5
        filterManager.applyFilter(to: layer)
        
        let appliedFilter = filterManager.appliedFilters.first!
        
        // Remove filter
        filterManager.removeFilter(appliedFilter)
        
        // Try to remove again
        filterManager.removeFilter(appliedFilter)
        
        XCTAssertEqual(filterManager.appliedFilters.count, 0, "Should still have no applied filters")
    }
    
    func testClearAllFilters() {
        let layer = createTestLayer()
        
        // Apply multiple filters
        filterManager.selectedFilter = .brightness
        filterManager.filterIntensity = 0.5
        filterManager.applyFilter(to: layer)
        
        filterManager.selectedFilter = .contrast
        filterManager.filterIntensity = 0.7
        filterManager.applyFilter(to: layer)
        
        XCTAssertEqual(filterManager.appliedFilters.count, 2, "Should have two applied filters")
        
        // Clear all filters
        filterManager.clearAllFilters()
        
        XCTAssertEqual(filterManager.appliedFilters.count, 0, "Should have no applied filters after clear")
    }
    
    // MARK: - Filter Preview Tests
    
    func testFilterPreview() {
        filterManager.selectedFilter = .brightness
        filterManager.filterIntensity = 0.5
        
        let layer = createTestLayer()
        let preview = filterManager.getFilterPreview(for: layer)
        
        XCTAssertNotNil(preview, "Filter preview should not be nil")
        XCTAssertEqual(preview.filter, .brightness, "Preview should have correct filter")
        XCTAssertEqual(preview.intensity, 0.5, "Preview should have correct intensity")
    }
    
    func testFilterPreviewWithMultipleFilters() {
        let layer = createTestLayer()
        
        // Apply multiple filters
        filterManager.selectedFilter = .brightness
        filterManager.filterIntensity = 0.5
        filterManager.applyFilter(to: layer)
        
        filterManager.selectedFilter = .contrast
        filterManager.filterIntensity = 0.7
        filterManager.applyFilter(to: layer)
        
        let preview = filterManager.getFilterPreview(for: layer)
        
        XCTAssertNotNil(preview, "Filter preview should not be nil")
        XCTAssertEqual(preview.appliedFilters.count, 2, "Preview should have two applied filters")
    }
    
    // MARK: - Filter Categories Tests
    
    func testFilterCategories() {
        let colorFilters = FilterCategory.color.filters
        let blurFilters = FilterCategory.blur.filters
        let distortionFilters = FilterCategory.distortion.filters
        let stylizeFilters = FilterCategory.stylize.filters
        let sharpenFilters = FilterCategory.sharpen.filters
        let noiseFilters = FilterCategory.noise.filters
        let artisticFilters = FilterCategory.artistic.filters
        
        XCTAssertGreaterThan(colorFilters.count, 0, "Color filters should not be empty")
        XCTAssertGreaterThan(blurFilters.count, 0, "Blur filters should not be empty")
        XCTAssertGreaterThan(distortionFilters.count, 0, "Distortion filters should not be empty")
        XCTAssertGreaterThan(stylizeFilters.count, 0, "Stylize filters should not be empty")
        XCTAssertGreaterThan(sharpenFilters.count, 0, "Sharpen filters should not be empty")
        XCTAssertGreaterThan(noiseFilters.count, 0, "Noise filters should not be empty")
        XCTAssertGreaterThan(artisticFilters.count, 0, "Artistic filters should not be empty")
    }
    
    func testFilterCategoryFilter() {
        let colorFilters = FilterCategory.color.filters
        let blurFilters = FilterCategory.blur.filters
        
        XCTAssertTrue(colorFilters.contains(.brightness), "Color filters should contain brightness")
        XCTAssertTrue(colorFilters.contains(.contrast), "Color filters should contain contrast")
        XCTAssertTrue(blurFilters.contains(.gaussianBlur), "Blur filters should contain gaussian blur")
        XCTAssertTrue(blurFilters.contains(.motionBlur), "Blur filters should contain motion blur")
    }
    
    // MARK: - Filter Combinations Tests
    
    func testFilterCombinations() {
        let layer = createTestLayer()
        
        // Apply multiple filters from different categories
        filterManager.selectedFilter = .brightness
        filterManager.filterIntensity = 0.5
        filterManager.applyFilter(to: layer)
        
        filterManager.selectedFilter = .gaussianBlur
        filterManager.filterIntensity = 0.3
        filterManager.applyFilter(to: layer)
        
        filterManager.selectedFilter = .emboss
        filterManager.filterIntensity = 0.7
        filterManager.applyFilter(to: layer)
        
        XCTAssertEqual(filterManager.appliedFilters.count, 3, "Should have three applied filters")
        
        let categories = filterManager.appliedFilters.map { $0.filter.category }
        XCTAssertTrue(categories.contains(.color), "Should have color filter")
        XCTAssertTrue(categories.contains(.blur), "Should have blur filter")
        XCTAssertTrue(categories.contains(.stylize), "Should have stylize filter")
    }
    
    func testFilterCombinationOrder() {
        let layer = createTestLayer()
        
        // Apply filters in specific order
        filterManager.selectedFilter = .brightness
        filterManager.filterIntensity = 0.5
        filterManager.applyFilter(to: layer)
        
        filterManager.selectedFilter = .contrast
        filterManager.filterIntensity = 0.7
        filterManager.applyFilter(to: layer)
        
        let appliedFilters = filterManager.appliedFilters
        XCTAssertEqual(appliedFilters[0].filter, .brightness, "First filter should be brightness")
        XCTAssertEqual(appliedFilters[1].filter, .contrast, "Second filter should be contrast")
    }
    
    // MARK: - Filter Performance Tests
    
    func testFilterPerformance() {
        let layer = createTestLayer()
        
        measure {
            for _ in 0..<100 {
                filterManager.selectedFilter = .brightness
                filterManager.filterIntensity = 0.5
                filterManager.applyFilter(to: layer)
                filterManager.clearAllFilters()
            }
        }
    }
    
    func testFilterPreviewPerformance() {
        let layer = createTestLayer()
        
        measure {
            for _ in 0..<1000 {
                filterManager.selectedFilter = .brightness
                filterManager.filterIntensity = 0.5
                _ = filterManager.getFilterPreview(for: layer)
            }
        }
    }
    
    func testFilterCombinationPerformance() {
        let layer = createTestLayer()
        
        measure {
            for _ in 0..<100 {
                filterManager.selectedFilter = .brightness
                filterManager.filterIntensity = 0.5
                filterManager.applyFilter(to: layer)
                
                filterManager.selectedFilter = .contrast
                filterManager.filterIntensity = 0.7
                filterManager.applyFilter(to: layer)
                
                filterManager.selectedFilter = .gaussianBlur
                filterManager.filterIntensity = 0.3
                filterManager.applyFilter(to: layer)
                
                filterManager.clearAllFilters()
            }
        }
    }
    
    // MARK: - Edge Cases Tests
    
    func testFilterWithNilLayer() {
        filterManager.selectedFilter = .brightness
        filterManager.filterIntensity = 0.5
        
        filterManager.applyFilter(to: nil)
        
        XCTAssertEqual(filterManager.appliedFilters.count, 0, "Filter should not be applied to nil layer")
    }
    
    func testFilterWithExtremeIntensity() {
        filterManager.selectedFilter = .brightness
        filterManager.filterIntensity = 1000.0
        
        let layer = createTestLayer()
        filterManager.applyFilter(to: layer)
        
        XCTAssertEqual(filterManager.appliedFilters.count, 1, "Filter should be applied with clamped intensity")
        XCTAssertEqual(filterManager.appliedFilters.first?.intensity, 1.0, "Intensity should be clamped to 1.0")
    }
    
    func testFilterWithNegativeIntensity() {
        filterManager.selectedFilter = .brightness
        filterManager.filterIntensity = -1000.0
        
        let layer = createTestLayer()
        filterManager.applyFilter(to: layer)
        
        XCTAssertEqual(filterManager.appliedFilters.count, 0, "Filter with negative intensity should not be applied")
    }
    
    func testFilterWithZeroIntensity() {
        filterManager.selectedFilter = .brightness
        filterManager.filterIntensity = 0.0
        
        let layer = createTestLayer()
        filterManager.applyFilter(to: layer)
        
        XCTAssertEqual(filterManager.appliedFilters.count, 0, "Filter with zero intensity should not be applied")
    }
    
    // MARK: - Helper Methods
    
    private func createTestLayer() -> DrawingLayer {
        return DrawingLayer(
            id: UUID(),
            name: "Test Layer",
            opacity: 1.0,
            blendingMode: .normal,
            isVisible: true,
            timestamp: Date()
        )
    }
}

// MARK: - Mock Objects for Testing

class MockFilterManager: FilterManager {
    var mockSelectedFilter: Filter = .none
    var mockFilterIntensity: Double = 1.0
    var mockIsPreviewMode = false
    var mockAppliedFilters: [AppliedFilter] = []
    
    override var selectedFilter: Filter {
        get { return mockSelectedFilter }
        set { mockSelectedFilter = newValue }
    }
    
    override var filterIntensity: Double {
        get { return mockFilterIntensity }
        set { mockFilterIntensity = max(0.0, min(1.0, newValue)) }
    }
    
    override var isPreviewMode: Bool {
        get { return mockIsPreviewMode }
        set { mockIsPreviewMode = newValue }
    }
    
    override var appliedFilters: [AppliedFilter] {
        return mockAppliedFilters
    }
    
    override func applyFilter(to layer: DrawingLayer?) {
        guard let layer = layer, filterIntensity > 0 else { return }
        
        let appliedFilter = AppliedFilter(
            id: UUID(),
            filter: mockSelectedFilter,
            intensity: mockFilterIntensity,
            layerId: layer.id,
            timestamp: Date()
        )
        mockAppliedFilters.append(appliedFilter)
    }
    
    override func removeFilter(_ filter: AppliedFilter) {
        mockAppliedFilters.removeAll { $0.id == filter.id }
    }
    
    override func clearAllFilters() {
        mockAppliedFilters.removeAll()
    }
    
    override func getFilterPreview(for layer: DrawingLayer?) -> FilterPreview {
        return FilterPreview(
            filter: mockSelectedFilter,
            intensity: mockFilterIntensity,
            appliedFilters: mockAppliedFilters,
            timestamp: Date()
        )
    }
}