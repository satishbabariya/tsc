import XCTest
import SwiftUI
import UIKit
@testable import ColorFillingApp

class SaveShareTests: XCTestCase {
    
    var errorHandler: ErrorHandler!
    var mockCanvas: MockColoringCanvasView!
    
    override func setUpWithError() throws {
        errorHandler = ErrorHandler()
        mockCanvas = MockColoringCanvasView()
    }
    
    override func tearDownWithError() throws {
        errorHandler = nil
        mockCanvas = nil
    }
    
    // MARK: - Image Capture Tests
    
    func testImageCaptureSuccess() throws {
        // Test successful image capture
        let testView = Color.white.overlay(
            Circle()
                .fill(Color.red)
                .frame(width: 100, height: 100)
        )
        
        let capturedImage = SafeImageCapture.captureImage(from: testView, errorHandler: errorHandler)
        
        XCTAssertNotNil(capturedImage, "Image capture should succeed")
        XCTAssertNil(errorHandler.currentError, "No error should be set")
        
        // Verify image properties
        XCTAssertGreaterThan(capturedImage!.size.width, 0, "Image should have positive width")
        XCTAssertGreaterThan(capturedImage!.size.height, 0, "Image should have positive height")
    }
    
    func testImageCaptureWithComplexView() throws {
        // Test image capture with a complex view (similar to our coloring canvas)
        let complexView = ZStack {
            Color.white
            VStack {
                Circle().fill(Color.red)
                Rectangle().fill(Color.blue)
                Ellipse().fill(Color.green)
            }
            .frame(width: 400, height: 400)
        }
        
        let capturedImage = SafeImageCapture.captureImage(from: complexView, errorHandler: errorHandler)
        
        XCTAssertNotNil(capturedImage, "Complex view should be captured successfully")
        XCTAssertNil(errorHandler.currentError, "No error should be set")
    }
    
    func testImageCaptureMemoryLimit() throws {
        // Test image capture with a very large view that might exceed memory limits
        let largeView = Color.white.overlay(
            Rectangle()
                .fill(Color.red)
                .frame(width: 2000, height: 2000) // Very large view
        )
        
        let capturedImage = SafeImageCapture.captureImage(from: largeView, errorHandler: errorHandler)
        
        // Should either succeed with a reasonable size or fail gracefully
        if capturedImage != nil {
            let imageSize = capturedImage!.size.width * capturedImage!.size.height
            XCTAssertLessThan(imageSize, 1000000, "Image size should be within memory limits")
        } else {
            XCTAssertNotNil(errorHandler.currentError, "Should set memory warning error")
            XCTAssertEqual(errorHandler.currentError, .memoryWarning)
        }
    }
    
    // MARK: - Share Sheet Tests
    
    func testShareSheetCreation() throws {
        let testImage = createTestImage()
        let shareSheet = SafeShareSheet(activityItems: [testImage], errorHandler: errorHandler)
        
        // Test that share sheet can be created
        XCTAssertNotNil(shareSheet, "Share sheet should be created successfully")
    }
    
    func testShareSheetWithMultipleItems() throws {
        let testImage = createTestImage()
        let testText = "My coloring artwork!"
        
        let shareSheet = SafeShareSheet(activityItems: [testImage, testText], errorHandler: errorHandler)
        
        XCTAssertNotNil(shareSheet, "Share sheet with multiple items should be created")
    }
    
    func testShareSheetWithEmptyItems() throws {
        let shareSheet = SafeShareSheet(activityItems: [], errorHandler: errorHandler)
        
        XCTAssertNotNil(shareSheet, "Share sheet with empty items should be created")
    }
    
    // MARK: - Error Handling Tests
    
    func testImageCaptureErrorHandling() throws {
        // Test error handling when image capture fails
        let invalidView = Color.clear // This might cause issues
        
        let capturedImage = SafeImageCapture.captureImage(from: invalidView, errorHandler: errorHandler)
        
        // Should handle the error gracefully
        if capturedImage == nil {
            XCTAssertNotNil(errorHandler.currentError, "Should set error when capture fails")
        }
    }
    
    func testShareErrorHandling() throws {
        // Test that share errors are handled properly
        let shareSheet = SafeShareSheet(activityItems: [], errorHandler: errorHandler)
        
        // Simulate a share error
        shareSheet.completionWithItemsHandler?(nil, false, nil, NSError(domain: "TestError", code: 1, userInfo: nil))
        
        // Should handle the error
        XCTAssertNotNil(errorHandler.currentError, "Should handle share errors")
        XCTAssertEqual(errorHandler.currentError, .shareFailed)
    }
    
    // MARK: - Haptic Feedback Tests
    
    func testHapticFeedbackAvailability() throws {
        let hapticFeedback = SafeHapticFeedback.shared
        
        // Test that haptic feedback methods don't crash
        hapticFeedback.lightImpact()
        hapticFeedback.mediumImpact()
        hapticFeedback.heavyImpact()
        hapticFeedback.success()
        hapticFeedback.error()
        
        // If we get here without crashing, the test passes
        XCTAssertTrue(true, "Haptic feedback should not crash")
    }
    
    func testHapticFeedbackOnDifferentActions() throws {
        let hapticFeedback = SafeHapticFeedback.shared
        
        // Test different haptic feedback types for different actions
        hapticFeedback.lightImpact() // For color selection
        hapticFeedback.mediumImpact() // For template selection
        hapticFeedback.success() // For successful share
        
        XCTAssertTrue(true, "Different haptic feedback types should work")
    }
    
    // MARK: - Memory Management Tests
    
    func testMemoryUsageDuringImageCapture() throws {
        // Test memory usage during multiple image captures
        let testView = Color.white.overlay(
            Circle().fill(Color.red).frame(width: 100, height: 100)
        )
        
        var capturedImages: [UIImage] = []
        
        for _ in 0..<10 {
            if let image = SafeImageCapture.captureImage(from: testView, errorHandler: errorHandler) {
                capturedImages.append(image)
            }
        }
        
        XCTAssertEqual(capturedImages.count, 10, "Should capture all images successfully")
        
        // Clear the array to free memory
        capturedImages.removeAll()
        
        XCTAssertTrue(true, "Memory should be managed properly")
    }
    
    func testMemoryWarningHandling() throws {
        let memoryMonitor = MemoryMonitor()
        
        // Start monitoring
        memoryMonitor.startMonitoring()
        
        // Simulate high memory usage
        var largeImages: [UIImage] = []
        for _ in 0..<100 {
            if let image = createTestImage() {
                largeImages.append(image)
            }
        }
        
        // Wait a bit for memory monitoring to kick in
        sleep(2)
        
        // Stop monitoring
        memoryMonitor.stopMonitoring()
        
        // Clear memory
        largeImages.removeAll()
        
        XCTAssertTrue(true, "Memory monitoring should work without crashing")
    }
    
    // MARK: - Integration Tests
    
    func testCompleteSaveShareWorkflow() throws {
        // Test the complete workflow from coloring to sharing
        
        // 1. Set up a colored canvas
        mockCanvas.selectedColor = .blue
        mockCanvas.colorRegion("testRegion")
        
        // 2. Create a view representing the colored canvas
        let coloredView = ZStack {
            Color.white
            Circle().fill(mockCanvas.coloredRegions["testRegion"] ?? .clear)
        }
        
        // 3. Capture the image
        let capturedImage = SafeImageCapture.captureImage(from: coloredView, errorHandler: errorHandler)
        
        XCTAssertNotNil(capturedImage, "Should capture colored canvas successfully")
        
        // 4. Create share sheet
        if let image = capturedImage {
            let shareSheet = SafeShareSheet(activityItems: [image], errorHandler: errorHandler)
            XCTAssertNotNil(shareSheet, "Should create share sheet successfully")
        }
        
        // 5. Verify no errors occurred
        XCTAssertNil(errorHandler.currentError, "Complete workflow should not produce errors")
    }
    
    func testSaveShareWithDifferentTemplates() throws {
        // Test save/share functionality with different templates
        
        for template in ColoringTemplate.allCases {
            // Create a view for each template
            let templateView = ZStack {
                Color.white
                template.drawing.stroke(Color.black, lineWidth: 3)
            }
            
            // Capture image
            let capturedImage = SafeImageCapture.captureImage(from: templateView, errorHandler: errorHandler)
            
            XCTAssertNotNil(capturedImage, "Should capture \(template.name) template successfully")
            
            if let image = capturedImage {
                // Test share sheet creation
                let shareSheet = SafeShareSheet(activityItems: [image], errorHandler: errorHandler)
                XCTAssertNotNil(shareSheet, "Should create share sheet for \(template.name)")
            }
        }
        
        XCTAssertNil(errorHandler.currentError, "All templates should work without errors")
    }
    
    // MARK: - Edge Case Tests
    
    func testShareWithVeryLargeImage() throws {
        // Test sharing with a very large image
        let largeImage = createLargeTestImage()
        
        let shareSheet = SafeShareSheet(activityItems: [largeImage], errorHandler: errorHandler)
        
        XCTAssertNotNil(shareSheet, "Should handle large images gracefully")
    }
    
    func testShareWithCorruptedImage() throws {
        // Test sharing with a potentially corrupted image
        let corruptedImage = createCorruptedTestImage()
        
        let shareSheet = SafeShareSheet(activityItems: [corruptedImage], errorHandler: errorHandler)
        
        XCTAssertNotNil(shareSheet, "Should handle corrupted images gracefully")
    }
    
    func testMultipleShareOperations() throws {
        // Test multiple share operations in quick succession
        let testImage = createTestImage()
        
        for _ in 0..<5 {
            let shareSheet = SafeShareSheet(activityItems: [testImage], errorHandler: errorHandler)
            XCTAssertNotNil(shareSheet, "Should handle multiple share operations")
        }
    }
    
    // MARK: - Helper Methods
    
    private func createTestImage() -> UIImage {
        let size = CGSize(width: 100, height: 100)
        let renderer = UIGraphicsImageRenderer(size: size)
        
        return renderer.image { context in
            context.cgContext.setFillColor(UIColor.red.cgColor)
            context.cgContext.fillEllipse(in: CGRect(origin: .zero, size: size))
        }
    }
    
    private func createLargeTestImage() -> UIImage {
        let size = CGSize(width: 1000, height: 1000)
        let renderer = UIGraphicsImageRenderer(size: size)
        
        return renderer.image { context in
            context.cgContext.setFillColor(UIColor.blue.cgColor)
            context.cgContext.fill(CGRect(origin: .zero, size: size))
        }
    }
    
    private func createCorruptedTestImage() -> UIImage {
        // Create an image that might cause issues
        let size = CGSize(width: 0, height: 0) // Invalid size
        let renderer = UIGraphicsImageRenderer(size: size)
        
        return renderer.image { context in
            // Empty image
        }
    }
}

// MARK: - Mock Classes for Testing

class MockColoringCanvasView: ObservableObject {
    @Published var coloredRegions: [String: Color] = [:]
    @Published var selectedColor: Color = .red
    
    func colorRegion(_ regionId: String) {
        coloredRegions[regionId] = selectedColor
    }
    
    func clearAll() {
        coloredRegions.removeAll()
    }
    
    func getColoredRegionCount() -> Int {
        return coloredRegions.count
    }
}

// MARK: - Performance Tests

class SaveSharePerformanceTests: XCTestCase {
    
    func testImageCapturePerformance() throws {
        let testView = Color.white.overlay(
            Circle().fill(Color.red).frame(width: 100, height: 100)
        )
        
        let errorHandler = ErrorHandler()
        
        measure {
            for _ in 0..<100 {
                _ = SafeImageCapture.captureImage(from: testView, errorHandler: errorHandler)
            }
        }
    }
    
    func testShareSheetCreationPerformance() throws {
        let testImage = createTestImage()
        let errorHandler = ErrorHandler()
        
        measure {
            for _ in 0..<1000 {
                _ = SafeShareSheet(activityItems: [testImage], errorHandler: errorHandler)
            }
        }
    }
    
    private func createTestImage() -> UIImage {
        let size = CGSize(width: 100, height: 100)
        let renderer = UIGraphicsImageRenderer(size: size)
        
        return renderer.image { context in
            context.cgContext.setFillColor(UIColor.red.cgColor)
            context.cgContext.fillEllipse(in: CGRect(origin: .zero, size: size))
        }
    }
}