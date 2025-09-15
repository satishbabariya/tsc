import XCTest
import SwiftUI
@testable import ColorFillingApp

class PerformanceTests: XCTestCase {
    
    // MARK: - Template Rendering Performance
    
    func testTemplateRenderingPerformance() throws {
        measure {
            for _ in 0..<1000 {
                for template in ColoringTemplate.allCases {
                    _ = template.drawing
                    _ = template.regions
                }
            }
        }
    }
    
    func testIndividualTemplatePerformance() throws {
        let templates = ColoringTemplate.allCases
        
        for template in templates {
            measure {
                for _ in 0..<1000 {
                    _ = template.drawing
                    _ = template.regions
                }
            }
        }
    }
    
    func testPathRenderingPerformance() throws {
        let butterfly = ButterflyTemplate()
        
        measure {
            for _ in 0..<10000 {
                _ = butterfly.path.boundingRect
            }
        }
    }
    
    // MARK: - Color Selection Performance
    
    func testColorSelectionPerformance() throws {
        let colors: [Color] = [
            .red, .orange, .yellow, .green, .mint, .teal,
            .cyan, .blue, .indigo, .purple, .pink, .brown,
            .gray, .black, .white, Color(red: 1, green: 0.8, blue: 0.8)
        ]
        
        measure {
            for _ in 0..<10000 {
                for color in colors {
                    _ = color.description
                }
            }
        }
    }
    
    func testColorComparisonPerformance() throws {
        let color1 = Color.red
        let color2 = Color.blue
        
        measure {
            for _ in 0..<100000 {
                _ = color1.description == color2.description
            }
        }
    }
    
    // MARK: - Animation Performance
    
    func testAnimationPerformance() throws {
        measure {
            for _ in 0..<100 {
                withAnimation(.spring(response: 0.3, dampingFraction: 0.6)) {
                    // Simulate animation
                    _ = Color.red
                }
            }
        }
    }
    
    func testMultipleAnimationsPerformance() throws {
        measure {
            for _ in 0..<50 {
                withAnimation(.spring(response: 0.3, dampingFraction: 0.6)) {
                    _ = Color.red
                }
                withAnimation(.easeInOut(duration: 0.5)) {
                    _ = Color.blue
                }
                withAnimation(.linear(duration: 0.2)) {
                    _ = Color.green
                }
            }
        }
    }
    
    // MARK: - Memory Usage Performance
    
    func testMemoryUsageDuringColoring() throws {
        var coloredRegions: [String: Color] = [:]
        
        measure {
            for i in 0..<1000 {
                coloredRegions["region\(i)"] = Color.red
            }
        }
        
        // Clear memory
        coloredRegions.removeAll()
    }
    
    func testMemoryUsageDuringTemplateSwitching() throws {
        measure {
            for _ in 0..<100 {
                for template in ColoringTemplate.allCases {
                    let regions = template.regions
                    let drawing = template.drawing
                    _ = regions
                    _ = drawing
                }
            }
        }
    }
    
    // MARK: - Image Capture Performance
    
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
    
    func testLargeImageCapturePerformance() throws {
        let largeView = Color.white.overlay(
            Rectangle().fill(Color.blue).frame(width: 500, height: 500)
        )
        
        let errorHandler = ErrorHandler()
        
        measure {
            for _ in 0..<50 {
                _ = SafeImageCapture.captureImage(from: largeView, errorHandler: errorHandler)
            }
        }
    }
    
    // MARK: - Haptic Feedback Performance
    
    func testHapticFeedbackPerformance() throws {
        let hapticFeedback = SafeHapticFeedback.shared
        
        measure {
            for _ in 0..<1000 {
                hapticFeedback.lightImpact()
            }
        }
    }
    
    func testMultipleHapticFeedbackPerformance() throws {
        let hapticFeedback = SafeHapticFeedback.shared
        
        measure {
            for _ in 0..<100 {
                hapticFeedback.lightImpact()
                hapticFeedback.mediumImpact()
                hapticFeedback.heavyImpact()
            }
        }
    }
    
    // MARK: - UI Rendering Performance
    
    func testUIRenderingPerformance() throws {
        let testView = VStack {
            ForEach(0..<100, id: \.self) { index in
                Circle()
                    .fill(Color.red)
                    .frame(width: 20, height: 20)
            }
        }
        
        measure {
            for _ in 0..<100 {
                _ = testView.body
            }
        }
    }
    
    func testComplexUIRenderingPerformance() throws {
        let complexView = VStack {
            ForEach(ColoringTemplate.allCases, id: \.self) { template in
                HStack {
                    template.icon
                    Text(template.name)
                    ForEach(template.regions, id: \.id) { region in
                        Circle().fill(Color.blue)
                    }
                }
            }
        }
        
        measure {
            for _ in 0..<50 {
                _ = complexView.body
            }
        }
    }
    
    // MARK: - Data Structure Performance
    
    func testDictionaryPerformance() throws {
        var coloredRegions: [String: Color] = [:]
        
        measure {
            for i in 0..<10000 {
                coloredRegions["region\(i)"] = Color.red
            }
        }
        
        coloredRegions.removeAll()
    }
    
    func testArrayPerformance() throws {
        var colors: [Color] = []
        
        measure {
            for _ in 0..<10000 {
                colors.append(Color.red)
            }
        }
        
        colors.removeAll()
    }
    
    func testSetPerformance() throws {
        var colorSet: Set<String> = []
        
        measure {
            for i in 0..<10000 {
                colorSet.insert("color\(i)")
            }
        }
        
        colorSet.removeAll()
    }
    
    // MARK: - String Performance
    
    func testStringPerformance() throws {
        measure {
            for i in 0..<10000 {
                _ = "region\(i)"
            }
        }
    }
    
    func testStringComparisonPerformance() throws {
        let string1 = "region1"
        let string2 = "region2"
        
        measure {
            for _ in 0..<100000 {
                _ = string1 == string2
            }
        }
    }
    
    // MARK: - Math Performance
    
    func testMathPerformance() throws {
        measure {
            for i in 0..<100000 {
                _ = sin(Double(i))
                _ = cos(Double(i))
                _ = sqrt(Double(i))
            }
        }
    }
    
    func testGeometryPerformance() throws {
        measure {
            for i in 0..<100000 {
                let point = CGPoint(x: Double(i), y: Double(i))
                let size = CGSize(width: Double(i), height: Double(i))
                let rect = CGRect(origin: point, size: size)
                _ = rect.width
                _ = rect.height
            }
        }
    }
    
    // MARK: - Concurrency Performance
    
    func testConcurrencyPerformance() throws {
        let expectation = XCTestExpectation(description: "Concurrency test")
        let group = DispatchGroup()
        
        measure {
            for _ in 0..<100 {
                group.enter()
                DispatchQueue.global().async {
                    // Simulate work
                    _ = Color.red
                    group.leave()
                }
            }
            
            group.notify(queue: .main) {
                expectation.fulfill()
            }
            
            wait(for: [expectation], timeout: 10.0)
        }
    }
    
    // MARK: - Memory Allocation Performance
    
    func testMemoryAllocationPerformance() throws {
        measure {
            for _ in 0..<1000 {
                var objects: [Any] = []
                for _ in 0..<100 {
                    objects.append(Color.red)
                }
                objects.removeAll()
            }
        }
    }
    
    func testMemoryDeallocationPerformance() throws {
        measure {
            for _ in 0..<1000 {
                var objects: [Any] = []
                for _ in 0..<100 {
                    objects.append(Color.red)
                }
                // objects will be deallocated when it goes out of scope
            }
        }
    }
    
    // MARK: - File I/O Performance
    
    func testFileIOPerformance() throws {
        let documentsPath = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask)[0]
        let testFileURL = documentsPath.appendingPathComponent("test_performance.txt")
        
        measure {
            for i in 0..<1000 {
                let data = "Test data \(i)".data(using: .utf8)!
                try? data.write(to: testFileURL)
            }
        }
        
        // Clean up
        try? FileManager.default.removeItem(at: testFileURL)
    }
    
    // MARK: - Network Performance (Mock)
    
    func testNetworkPerformance() throws {
        measure {
            for _ in 0..<100 {
                // Simulate network request
                let url = URL(string: "https://example.com")!
                let request = URLRequest(url: url)
                _ = request
            }
        }
    }
    
    // MARK: - Database Performance (Mock)
    
    func testDatabasePerformance() throws {
        measure {
            for i in 0..<1000 {
                // Simulate database operations
                let data = ["id": i, "color": "red", "region": "region\(i)"]
                _ = data
            }
        }
    }
    
    // MARK: - Integration Performance Tests
    
    func testCompleteAppPerformance() throws {
        measure {
            for _ in 0..<100 {
                // Simulate complete app workflow
                
                // 1. Template selection
                for template in ColoringTemplate.allCases {
                    _ = template.drawing
                    _ = template.regions
                }
                
                // 2. Color selection
                let colors: [Color] = [.red, .blue, .green, .yellow]
                for color in colors {
                    _ = color.description
                }
                
                // 3. Coloring regions
                var coloredRegions: [String: Color] = [:]
                for i in 0..<10 {
                    coloredRegions["region\(i)"] = Color.red
                }
                
                // 4. Image capture
                let testView = Color.white.overlay(Circle().fill(Color.red))
                let errorHandler = ErrorHandler()
                _ = SafeImageCapture.captureImage(from: testView, errorHandler: errorHandler)
                
                // 5. Haptic feedback
                let hapticFeedback = SafeHapticFeedback.shared
                hapticFeedback.lightImpact()
                
                // Clear memory
                coloredRegions.removeAll()
            }
        }
    }
    
    func testStressTestPerformance() throws {
        measure {
            for _ in 0..<10 {
                // Stress test with many operations
                
                // Many template operations
                for _ in 0..<100 {
                    for template in ColoringTemplate.allCases {
                        _ = template.drawing
                        _ = template.regions
                    }
                }
                
                // Many color operations
                for _ in 0..<100 {
                    let colors: [Color] = [.red, .blue, .green, .yellow, .purple, .orange]
                    for color in colors {
                        _ = color.description
                    }
                }
                
                // Many memory operations
                var coloredRegions: [String: Color] = [:]
                for i in 0..<1000 {
                    coloredRegions["region\(i)"] = Color.red
                }
                coloredRegions.removeAll()
                
                // Many haptic operations
                let hapticFeedback = SafeHapticFeedback.shared
                for _ in 0..<100 {
                    hapticFeedback.lightImpact()
                }
            }
        }
    }
}

// MARK: - Performance Monitoring

class PerformanceMonitor {
    static let shared = PerformanceMonitor()
    
    private var startTime: CFAbsoluteTime = 0
    private var endTime: CFAbsoluteTime = 0
    
    func startTiming() {
        startTime = CFAbsoluteTimeGetCurrent()
    }
    
    func endTiming() -> TimeInterval {
        endTime = CFAbsoluteTimeGetCurrent()
        return endTime - startTime
    }
    
    func measureTime<T>(_ operation: () throws -> T) rethrows -> (result: T, time: TimeInterval) {
        startTiming()
        let result = try operation()
        let time = endTiming()
        return (result, time)
    }
}

// MARK: - Performance Optimization Tests

class PerformanceOptimizationTests: XCTestCase {
    
    func testOptimizedTemplateRendering() throws {
        // Test optimized template rendering
        let monitor = PerformanceMonitor.shared
        
        let (_, time) = monitor.measureTime {
            for _ in 0..<1000 {
                for template in ColoringTemplate.allCases {
                    _ = template.drawing
                    _ = template.regions
                }
            }
        }
        
        // Should complete within reasonable time
        XCTAssertLessThan(time, 1.0, "Optimized template rendering should be fast")
    }
    
    func testOptimizedColorSelection() throws {
        // Test optimized color selection
        let monitor = PerformanceMonitor.shared
        
        let (_, time) = monitor.measureTime {
            for _ in 0..<10000 {
                let colors: [Color] = [.red, .blue, .green, .yellow]
                for color in colors {
                    _ = color.description
                }
            }
        }
        
        // Should complete within reasonable time
        XCTAssertLessThan(time, 1.0, "Optimized color selection should be fast")
    }
    
    func testOptimizedMemoryUsage() throws {
        // Test optimized memory usage
        let monitor = PerformanceMonitor.shared
        
        let (_, time) = monitor.measureTime {
            var coloredRegions: [String: Color] = [:]
            for i in 0..<10000 {
                coloredRegions["region\(i)"] = Color.red
            }
            coloredRegions.removeAll()
        }
        
        // Should complete within reasonable time
        XCTAssertLessThan(time, 1.0, "Optimized memory usage should be fast")
    }
}