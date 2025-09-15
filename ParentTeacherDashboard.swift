import SwiftUI

// MARK: - Dashboard Manager

class DashboardManager: ObservableObject {
    @Published var showDashboard = false
    @Published var selectedTab: DashboardTab = .overview
    @Published var childProfiles: [ChildProfile] = []
    @Published var currentChild: ChildProfile?
    @Published var showAddChildDialog = false
    @Published var showSettingsDialog = false
    
    init() {
        loadChildProfiles()
    }
    
    func addChild(_ child: ChildProfile) {
        childProfiles.append(child)
        saveChildProfiles()
    }
    
    func removeChild(_ child: ChildProfile) {
        childProfiles.removeAll { $0.id == child.id }
        saveChildProfiles()
    }
    
    func selectChild(_ child: ChildProfile) {
        currentChild = child
    }
    
    private func saveChildProfiles() {
        if let encoded = try? JSONEncoder().encode(childProfiles) {
            UserDefaults.standard.set(encoded, forKey: "childProfiles")
        }
    }
    
    private func loadChildProfiles() {
        if let data = UserDefaults.standard.data(forKey: "childProfiles"),
           let decoded = try? JSONDecoder().decode([ChildProfile].self, from: data) {
            childProfiles = decoded
        }
    }
}

enum DashboardTab: String, CaseIterable {
    case overview = "Overview"
    case progress = "Progress"
    case achievements = "Achievements"
    case artwork = "Artwork"
    case settings = "Settings"
    
    var icon: Image {
        switch self {
        case .overview: return Image(systemName: "house.fill")
        case .progress: return Image(systemName: "chart.line.uptrend.xyaxis")
        case .achievements: return Image(systemName: "trophy.fill")
        case .artwork: return Image(systemName: "photo.fill")
        case .settings: return Image(systemName: "gear")
        }
    }
}

// MARK: - Child Profile Model

struct ChildProfile: Identifiable, Codable {
    let id = UUID()
    let name: String
    let age: Int
    let avatar: String
    let createdAt: Date
    var statistics: ChildStatistics
    var preferences: ChildPreferences
    
    init(name: String, age: Int, avatar: String) {
        self.name = name
        self.age = age
        self.avatar = avatar
        self.createdAt = Date()
        self.statistics = ChildStatistics()
        self.preferences = ChildPreferences()
    }
}

struct ChildStatistics: Codable {
    var totalSessionTime: TimeInterval = 0
    var totalArtworksCreated: Int = 0
    var totalRegionsColored: Int = 0
    var totalStickersAdded: Int = 0
    var favoriteTemplate: String = "butterfly"
    var favoriteColor: String = "red"
    var averageSessionTime: TimeInterval = 0
    var sessionsCount: Int = 0
    var lastActivityDate: Date = Date()
    var consecutiveDays: Int = 0
    var achievementsUnlocked: Int = 0
    var level: Int = 1
    var experience: Int = 0
}

struct ChildPreferences: Codable {
    var soundEnabled: Bool = true
    var musicEnabled: Bool = true
    var hapticEnabled: Bool = true
    var preferredMusicTrack: String = "happy"
    var soundVolume: Float = 0.7
    var musicVolume: Float = 0.5
    var autoSave: Bool = true
    var showHints: Bool = true
    var difficultyLevel: DifficultyLevel = .easy
}

enum DifficultyLevel: String, CaseIterable, Codable {
    case easy = "Easy"
    case medium = "Medium"
    case hard = "Hard"
    
    var description: String {
        switch self {
        case .easy: return "Simple templates with large regions"
        case .medium: return "Medium complexity with moderate regions"
        case .hard: return "Complex templates with small regions"
        }
    }
}

// MARK: - Parent/Teacher Dashboard View

struct ParentTeacherDashboardView: View {
    @ObservedObject var dashboardManager: DashboardManager
    @ObservedObject var progressTracker: ProgressTracker
    @ObservedObject var artworkManager: ArtworkManager
    
    var body: some View {
        NavigationView {
            VStack(spacing: 0) {
                // Header
                HStack {
                    Text("👨‍👩‍👧‍👦 Parent Dashboard")
                        .font(.title)
                        .fontWeight(.bold)
                        .foregroundColor(.primary)
                    
                    Spacer()
                    
                    Button(action: {
                        dashboardManager.showAddChildDialog = true
                    }) {
                        Image(systemName: "person.badge.plus")
                            .foregroundColor(.blue)
                            .font(.system(size: 20))
                    }
                }
                .padding()
                .background(Color.gray.opacity(0.1))
                
                // Child selector
                if !dashboardManager.childProfiles.isEmpty {
                    ScrollView(.horizontal, showsIndicators: false) {
                        HStack(spacing: 15) {
                            ForEach(dashboardManager.childProfiles) { child in
                                ChildProfileCard(
                                    child: child,
                                    isSelected: dashboardManager.currentChild?.id == child.id,
                                    onSelect: {
                                        dashboardManager.selectChild(child)
                                    }
                                )
                            }
                        }
                        .padding(.horizontal)
                    }
                    .padding(.vertical, 10)
                }
                
                // Tab selector
                ScrollView(.horizontal, showsIndicators: false) {
                    HStack(spacing: 20) {
                        ForEach(DashboardTab.allCases, id: \.self) { tab in
                            Button(action: {
                                dashboardManager.selectedTab = tab
                            }) {
                                VStack {
                                    tab.icon
                                        .font(.system(size: 20))
                                        .foregroundColor(dashboardManager.selectedTab == tab ? .white : .primary)
                                    
                                    Text(tab.rawValue)
                                        .font(.caption)
                                        .foregroundColor(dashboardManager.selectedTab == tab ? .white : .primary)
                                }
                                .padding(.horizontal, 15)
                                .padding(.vertical, 10)
                                .background(
                                    RoundedRectangle(cornerRadius: 10)
                                        .fill(dashboardManager.selectedTab == tab ? Color.blue : Color.gray.opacity(0.2))
                                )
                            }
                            .buttonStyle(PlainButtonStyle())
                        }
                    }
                    .padding(.horizontal)
                }
                .padding(.vertical, 10)
                
                // Content
                ScrollView {
                    switch dashboardManager.selectedTab {
                    case .overview:
                        OverviewTabView(
                            child: dashboardManager.currentChild,
                            progressTracker: progressTracker,
                            artworkManager: artworkManager
                        )
                    case .progress:
                        ProgressTabView(
                            child: dashboardManager.currentChild,
                            progressTracker: progressTracker
                        )
                    case .achievements:
                        AchievementsTabView(
                            child: dashboardManager.currentChild,
                            progressTracker: progressTracker
                        )
                    case .artwork:
                        ArtworkTabView(
                            child: dashboardManager.currentChild,
                            artworkManager: artworkManager
                        )
                    case .settings:
                        SettingsTabView(
                            child: dashboardManager.currentChild,
                            dashboardManager: dashboardManager
                        )
                    }
                }
            }
            .navigationBarHidden(true)
        }
        .sheet(isPresented: $dashboardManager.showAddChildDialog) {
            AddChildDialogView(dashboardManager: dashboardManager)
        }
        .sheet(isPresented: $dashboardManager.showSettingsDialog) {
            SettingsDialogView(dashboardManager: dashboardManager)
        }
    }
}

// MARK: - Child Profile Card

struct ChildProfileCard: View {
    let child: ChildProfile
    let isSelected: Bool
    let onSelect: () -> Void
    
    var body: some View {
        Button(action: onSelect) {
            VStack(spacing: 10) {
                // Avatar
                ZStack {
                    Circle()
                        .fill(isSelected ? Color.blue : Color.gray.opacity(0.3))
                        .frame(width: 60, height: 60)
                    
                    Text(child.avatar)
                        .font(.system(size: 30))
                }
                
                // Name
                Text(child.name)
                    .font(.headline)
                    .foregroundColor(.primary)
                
                // Age
                Text("Age \(child.age)")
                    .font(.caption)
                    .foregroundColor(.secondary)
                
                // Level
                Text("Level \(child.statistics.level)")
                    .font(.caption)
                    .foregroundColor(.blue)
            }
            .padding()
            .background(
                RoundedRectangle(cornerRadius: 15)
                    .fill(isSelected ? Color.blue.opacity(0.1) : Color.gray.opacity(0.05))
            )
            .overlay(
                RoundedRectangle(cornerRadius: 15)
                    .stroke(isSelected ? Color.blue : Color.clear, lineWidth: 2)
            )
        }
        .buttonStyle(PlainButtonStyle())
    }
}

// MARK: - Overview Tab View

struct OverviewTabView: View {
    let child: ChildProfile?
    @ObservedObject var progressTracker: ProgressTracker
    @ObservedObject var artworkManager: ArtworkManager
    
    var body: some View {
        VStack(spacing: 20) {
            if let child = child {
                // Child overview
                VStack(spacing: 15) {
                    Text("👋 Welcome back, \(child.name)!")
                        .font(.title2)
                        .fontWeight(.bold)
                        .foregroundColor(.primary)
                    
                    // Quick stats
                    LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 2), spacing: 15) {
                        StatCardView(
                            title: "Artworks Created",
                            value: "\(child.statistics.totalArtworksCreated)",
                            icon: "photo.fill",
                            color: .blue
                        )
                        
                        StatCardView(
                            title: "Regions Colored",
                            value: "\(child.statistics.totalRegionsColored)",
                            icon: "paintbrush.fill",
                            color: .green
                        )
                        
                        StatCardView(
                            title: "Stickers Added",
                            value: "\(child.statistics.totalStickersAdded)",
                            icon: "sparkles",
                            color: .purple
                        )
                        
                        StatCardView(
                            title: "Achievements",
                            value: "\(child.statistics.achievementsUnlocked)",
                            icon: "trophy.fill",
                            color: .orange
                        )
                    }
                    
                    // Recent activity
                    VStack(alignment: .leading, spacing: 10) {
                        Text("Recent Activity")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        VStack(alignment: .leading, spacing: 5) {
                            Text("Last activity: \(child.statistics.lastActivityDate, style: .relative)")
                                .font(.subheadline)
                                .foregroundColor(.secondary)
                            
                            Text("Consecutive days: \(child.statistics.consecutiveDays)")
                                .font(.subheadline)
                                .foregroundColor(.secondary)
                            
                            Text("Total session time: \(formatTimeInterval(child.statistics.totalSessionTime))")
                                .font(.subheadline)
                                .foregroundColor(.secondary)
                        }
                    }
                    .padding()
                    .background(Color.gray.opacity(0.1))
                    .cornerRadius(10)
                }
            } else {
                // No child selected
                VStack(spacing: 20) {
                    Image(systemName: "person.circle")
                        .font(.system(size: 80))
                        .foregroundColor(.gray)
                    
                    Text("No Child Selected")
                        .font(.title2)
                        .fontWeight(.bold)
                        .foregroundColor(.primary)
                    
                    Text("Select a child profile to view their progress and achievements")
                        .font(.subheadline)
                        .foregroundColor(.secondary)
                        .multilineTextAlignment(.center)
                }
                .padding()
            }
        }
        .padding()
    }
    
    private func formatTimeInterval(_ interval: TimeInterval) -> String {
        let hours = Int(interval) / 3600
        let minutes = Int(interval) % 3600 / 60
        
        if hours > 0 {
            return "\(hours)h \(minutes)m"
        } else {
            return "\(minutes)m"
        }
    }
}

// MARK: - Progress Tab View

struct ProgressTabView: View {
    let child: ChildProfile?
    @ObservedObject var progressTracker: ProgressTracker
    
    var body: some View {
        VStack(spacing: 20) {
            if let child = child {
                Text("📈 \(child.name)'s Progress")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                // Progress charts would go here
                VStack(spacing: 15) {
                    ProgressChartView(
                        title: "Artworks Created Over Time",
                        data: generateSampleData()
                    )
                    
                    ProgressChartView(
                        title: "Session Time Trends",
                        data: generateSampleData()
                    )
                }
            } else {
                Text("Select a child to view progress")
                    .font(.title2)
                    .foregroundColor(.secondary)
            }
        }
        .padding()
    }
    
    private func generateSampleData() -> [ChartDataPoint] {
        return [
            ChartDataPoint(label: "Mon", value: 3),
            ChartDataPoint(label: "Tue", value: 5),
            ChartDataPoint(label: "Wed", value: 2),
            ChartDataPoint(label: "Thu", value: 7),
            ChartDataPoint(label: "Fri", value: 4),
            ChartDataPoint(label: "Sat", value: 6),
            ChartDataPoint(label: "Sun", value: 8)
        ]
    }
}

struct ChartDataPoint {
    let label: String
    let value: Double
}

struct ProgressChartView: View {
    let title: String
    let data: [ChartDataPoint]
    
    var body: some View {
        VStack(alignment: .leading, spacing: 10) {
            Text(title)
                .font(.headline)
                .foregroundColor(.primary)
            
            HStack(alignment: .bottom, spacing: 10) {
                ForEach(Array(data.enumerated()), id: \.offset) { index, point in
                    VStack {
                        RoundedRectangle(cornerRadius: 4)
                            .fill(Color.blue)
                            .frame(width: 30, height: CGFloat(point.value * 10))
                        
                        Text(point.label)
                            .font(.caption)
                            .foregroundColor(.secondary)
                    }
                }
            }
            .frame(height: 150)
        }
        .padding()
        .background(Color.gray.opacity(0.1))
        .cornerRadius(10)
    }
}

// MARK: - Achievements Tab View

struct AchievementsTabView: View {
    let child: ChildProfile?
    @ObservedObject var progressTracker: ProgressTracker
    
    var body: some View {
        VStack(spacing: 20) {
            if let child = child {
                Text("🏆 \(child.name)'s Achievements")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                AchievementsView(progressTracker: progressTracker)
            } else {
                Text("Select a child to view achievements")
                    .font(.title2)
                    .foregroundColor(.secondary)
            }
        }
        .padding()
    }
}

// MARK: - Artwork Tab View

struct ArtworkTabView: View {
    let child: ChildProfile?
    @ObservedObject var artworkManager: ArtworkManager
    
    var body: some View {
        VStack(spacing: 20) {
            if let child = child {
                Text("🖼️ \(child.name)'s Artwork")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                ArtworkGalleryView(artworkManager: artworkManager)
            } else {
                Text("Select a child to view artwork")
                    .font(.title2)
                    .foregroundColor(.secondary)
            }
        }
        .padding()
    }
}

// MARK: - Settings Tab View

struct SettingsTabView: View {
    let child: ChildProfile?
    @ObservedObject var dashboardManager: DashboardManager
    
    var body: some View {
        VStack(spacing: 20) {
            Text("⚙️ Settings")
                .font(.title2)
                .fontWeight(.bold)
                .foregroundColor(.primary)
            
            if let child = child {
                VStack(spacing: 15) {
                    Text("Child Settings: \(child.name)")
                        .font(.headline)
                        .foregroundColor(.primary)
                    
                    // Child preferences
                    VStack(alignment: .leading, spacing: 10) {
                        Toggle("Sound Effects", isOn: Binding(
                            get: { child.preferences.soundEnabled },
                            set: { _ in }
                        ))
                        
                        Toggle("Background Music", isOn: Binding(
                            get: { child.preferences.musicEnabled },
                            set: { _ in }
                        ))
                        
                        Toggle("Haptic Feedback", isOn: Binding(
                            get: { child.preferences.hapticEnabled },
                            set: { _ in }
                        ))
                        
                        Toggle("Auto Save", isOn: Binding(
                            get: { child.preferences.autoSave },
                            set: { _ in }
                        ))
                        
                        Toggle("Show Hints", isOn: Binding(
                            get: { child.preferences.showHints },
                            set: { _ in }
                        ))
                    }
                    .padding()
                    .background(Color.gray.opacity(0.1))
                    .cornerRadius(10)
                }
            }
            
            // App settings
            VStack(spacing: 15) {
                Text("App Settings")
                    .font(.headline)
                    .foregroundColor(.primary)
                
                VStack(spacing: 10) {
                    Button("Manage Child Profiles") {
                        dashboardManager.showAddChildDialog = true
                    }
                    .buttonStyle(.bordered)
                    
                    Button("Export Data") {
                        // Export functionality
                    }
                    .buttonStyle(.bordered)
                    
                    Button("Reset All Data", role: .destructive) {
                        // Reset functionality
                    }
                    .buttonStyle(.bordered)
                }
            }
            .padding()
            .background(Color.gray.opacity(0.1))
            .cornerRadius(10)
        }
        .padding()
    }
}

// MARK: - Add Child Dialog

struct AddChildDialogView: View {
    @ObservedObject var dashboardManager: DashboardManager
    @Environment(\.dismiss) private var dismiss
    @State private var childName = ""
    @State private var childAge = 5
    @State private var selectedAvatar = "👶"
    
    let avatars = ["👶", "🧒", "👧", "👦", "👨", "👩", "🧑", "👴", "👵"]
    
    var body: some View {
        NavigationView {
            VStack(spacing: 20) {
                Text("Add New Child Profile")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                VStack(spacing: 15) {
                    // Name
                    VStack(alignment: .leading) {
                        Text("Child's Name")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        TextField("Enter name", text: $childName)
                            .textFieldStyle(RoundedBorderTextFieldStyle())
                    }
                    
                    // Age
                    VStack(alignment: .leading) {
                        Text("Age")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        Stepper("\(childAge) years old", value: $childAge, in: 3...12)
                    }
                    
                    // Avatar
                    VStack(alignment: .leading) {
                        Text("Choose Avatar")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 3), spacing: 10) {
                            ForEach(avatars, id: \.self) { avatar in
                                Button(action: {
                                    selectedAvatar = avatar
                                }) {
                                    Text(avatar)
                                        .font(.system(size: 40))
                                        .frame(width: 60, height: 60)
                                        .background(
                                            Circle()
                                                .fill(selectedAvatar == avatar ? Color.blue.opacity(0.3) : Color.gray.opacity(0.1))
                                        )
                                }
                                .buttonStyle(PlainButtonStyle())
                            }
                        }
                    }
                }
                
                Spacer()
                
                HStack(spacing: 15) {
                    Button("Cancel") {
                        dismiss()
                    }
                    .buttonStyle(.bordered)
                    
                    Button("Add Child") {
                        let newChild = ChildProfile(
                            name: childName,
                            age: childAge,
                            avatar: selectedAvatar
                        )
                        dashboardManager.addChild(newChild)
                        dismiss()
                    }
                    .buttonStyle(.borderedProminent)
                    .disabled(childName.isEmpty)
                }
            }
            .padding()
            .navigationBarHidden(true)
        }
    }
}

// MARK: - Settings Dialog

struct SettingsDialogView: View {
    @ObservedObject var dashboardManager: DashboardManager
    @Environment(\.dismiss) private var dismiss
    
    var body: some View {
        NavigationView {
            VStack(spacing: 20) {
                Text("App Settings")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                VStack(spacing: 15) {
                    // General settings
                    VStack(alignment: .leading, spacing: 10) {
                        Text("General")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        Toggle("Enable Analytics", isOn: .constant(true))
                        Toggle("Auto Backup", isOn: .constant(true))
                        Toggle("Show Notifications", isOn: .constant(true))
                    }
                    
                    // Privacy settings
                    VStack(alignment: .leading, spacing: 10) {
                        Text("Privacy")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        Toggle("Share Usage Data", isOn: .constant(false))
                        Toggle("Allow Screenshots", isOn: .constant(true))
                    }
                }
                
                Spacer()
                
                Button("Done") {
                    dismiss()
                }
                .buttonStyle(.borderedProminent)
            }
            .padding()
            .navigationBarHidden(true)
        }
    }
}