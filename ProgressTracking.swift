import SwiftUI

// MARK: - Achievement Types

enum AchievementType: String, CaseIterable {
    case firstColor = "first_color"
    case colorMaster = "color_master"
    case templateExplorer = "template_explorer"
    case stickerArtist = "sticker_artist"
    case colorMixer = "color_mixer"
    case speedPainter = "speed_painter"
    case perfectionist = "perfectionist"
    case creativeGenius = "creative_genius"
    case dailyArtist = "daily_artist"
    case weekWarrior = "week_warrior"
    case monthMaster = "month_master"
    case yearLegend = "year_legend"
    
    var name: String {
        switch self {
        case .firstColor: return "First Color"
        case .colorMaster: return "Color Master"
        case .templateExplorer: return "Template Explorer"
        case .stickerArtist: return "Sticker Artist"
        case .colorMixer: return "Color Mixer"
        case .speedPainter: return "Speed Painter"
        case .perfectionist: return "Perfectionist"
        case .creativeGenius: return "Creative Genius"
        case .dailyArtist: return "Daily Artist"
        case .weekWarrior: return "Week Warrior"
        case .monthMaster: return "Month Master"
        case .yearLegend: return "Year Legend"
        }
    }
    
    var description: String {
        switch self {
        case .firstColor: return "Color your first region"
        case .colorMaster: return "Use 50 different colors"
        case .templateExplorer: return "Try all 18 templates"
        case .stickerArtist: return "Add 20 stickers to your artwork"
        case .colorMixer: return "Create 10 custom colors"
        case .speedPainter: return "Complete a template in under 2 minutes"
        case .perfectionist: return "Complete a template with perfect accuracy"
        case .creativeGenius: return "Create 100 unique artworks"
        case .dailyArtist: return "Use the app for 7 consecutive days"
        case .weekWarrior: return "Use the app for 30 consecutive days"
        case .monthMaster: return "Use the app for 90 consecutive days"
        case .yearLegend: return "Use the app for 365 consecutive days"
        }
    }
    
    var icon: Image {
        switch self {
        case .firstColor: return Image(systemName: "paintbrush.fill")
        case .colorMaster: return Image(systemName: "paintpalette.fill")
        case .templateExplorer: return Image(systemName: "square.grid.3x3.fill")
        case .stickerArtist: return Image(systemName: "sparkles")
        case .colorMixer: return Image(systemName: "drop.fill")
        case .speedPainter: return Image(systemName: "timer")
        case .perfectionist: return Image(systemName: "checkmark.seal.fill")
        case .creativeGenius: return Image(systemName: "lightbulb.fill")
        case .dailyArtist: return Image(systemName: "calendar")
        case .weekWarrior: return Image(systemName: "calendar.badge.clock")
        case .monthMaster: return Image(systemName: "calendar.badge.plus")
        case .yearLegend: return Image(systemName: "crown.fill")
        }
    }
    
    var rarity: AchievementRarity {
        switch self {
        case .firstColor: return .common
        case .colorMaster: return .uncommon
        case .templateExplorer: return .rare
        case .stickerArtist: return .uncommon
        case .colorMixer: return .rare
        case .speedPainter: return .epic
        case .perfectionist: return .epic
        case .creativeGenius: return .legendary
        case .dailyArtist: return .uncommon
        case .weekWarrior: return .rare
        case .monthMaster: return .epic
        case .yearLegend: return .legendary
        }
    }
    
    var points: Int {
        switch rarity {
        case .common: return 10
        case .uncommon: return 25
        case .rare: return 50
        case .epic: return 100
        case .legendary: return 250
        }
    }
}

enum AchievementRarity: String, CaseIterable {
    case common = "Common"
    case uncommon = "Uncommon"
    case rare = "Rare"
    case epic = "Epic"
    case legendary = "Legendary"
    
    var color: Color {
        switch self {
        case .common: return .gray
        case .uncommon: return .green
        case .rare: return .blue
        case .epic: return .purple
        case .legendary: return .orange
        }
    }
}

// MARK: - Achievement Model

struct Achievement: Identifiable, Codable {
    let id = UUID()
    let type: AchievementType
    let unlockedAt: Date
    let progress: Int
    let target: Int
    
    var isCompleted: Bool {
        return progress >= target
    }
    
    var progressPercentage: Double {
        return min(1.0, Double(progress) / Double(target))
    }
}

// MARK: - Progress Tracker

class ProgressTracker: ObservableObject {
    @Published var achievements: [Achievement] = []
    @Published var totalPoints: Int = 0
    @Published var level: Int = 1
    @Published var experience: Int = 0
    @Published var statistics: UserStatistics = UserStatistics()
    
    private let experiencePerLevel = 100
    
    init() {
        loadProgress()
    }
    
    func recordAction(_ action: UserAction) {
        statistics.recordAction(action)
        checkAchievements()
        updateLevel()
        saveProgress()
    }
    
    private func checkAchievements() {
        for achievementType in AchievementType.allCases {
            if !hasAchievement(achievementType) {
                let progress = getProgressForAchievement(achievementType)
                let target = getTargetForAchievement(achievementType)
                
                if progress >= target {
                    unlockAchievement(achievementType, progress: progress, target: target)
                }
            }
        }
    }
    
    private func hasAchievement(_ type: AchievementType) -> Bool {
        return achievements.contains { $0.type == type }
    }
    
    private func getProgressForAchievement(_ type: AchievementType) -> Int {
        switch type {
        case .firstColor:
            return statistics.totalRegionsColored > 0 ? 1 : 0
        case .colorMaster:
            return statistics.uniqueColorsUsed
        case .templateExplorer:
            return statistics.templatesUsed.count
        case .stickerArtist:
            return statistics.totalStickersAdded
        case .colorMixer:
            return statistics.customColorsCreated
        case .speedPainter:
            return statistics.fastCompletions
        case .perfectionist:
            return statistics.perfectCompletions
        case .creativeGenius:
            return statistics.totalArtworksCreated
        case .dailyArtist:
            return statistics.consecutiveDays
        case .weekWarrior:
            return statistics.consecutiveDays
        case .monthMaster:
            return statistics.consecutiveDays
        case .yearLegend:
            return statistics.consecutiveDays
        }
    }
    
    private func getTargetForAchievement(_ type: AchievementType) -> Int {
        switch type {
        case .firstColor: return 1
        case .colorMaster: return 50
        case .templateExplorer: return 18
        case .stickerArtist: return 20
        case .colorMixer: return 10
        case .speedPainter: return 1
        case .perfectionist: return 1
        case .creativeGenius: return 100
        case .dailyArtist: return 7
        case .weekWarrior: return 30
        case .monthMaster: return 90
        case .yearLegend: return 365
        }
    }
    
    private func unlockAchievement(_ type: AchievementType, progress: Int, target: Int) {
        let achievement = Achievement(
            type: type,
            unlockedAt: Date(),
            progress: progress,
            target: target
        )
        
        achievements.append(achievement)
        totalPoints += type.points
        
        // Play achievement sound
        EnhancedHapticFeedback.shared.achievement()
    }
    
    private func updateLevel() {
        let newLevel = (experience / experiencePerLevel) + 1
        if newLevel > level {
            level = newLevel
            // Level up sound effect could be added here
        }
    }
    
    private func saveProgress() {
        if let encoded = try? JSONEncoder().encode(statistics) {
            UserDefaults.standard.set(encoded, forKey: "userStatistics")
        }
        
        if let encoded = try? JSONEncoder().encode(achievements) {
            UserDefaults.standard.set(encoded, forKey: "achievements")
        }
        
        UserDefaults.standard.set(totalPoints, forKey: "totalPoints")
        UserDefaults.standard.set(level, forKey: "level")
        UserDefaults.standard.set(experience, forKey: "experience")
    }
    
    private func loadProgress() {
        if let data = UserDefaults.standard.data(forKey: "userStatistics"),
           let decoded = try? JSONDecoder().decode(UserStatistics.self, from: data) {
            statistics = decoded
        }
        
        if let data = UserDefaults.standard.data(forKey: "achievements"),
           let decoded = try? JSONDecoder().decode([Achievement].self, from: data) {
            achievements = decoded
        }
        
        totalPoints = UserDefaults.standard.integer(forKey: "totalPoints")
        level = UserDefaults.standard.integer(forKey: "level")
        experience = UserDefaults.standard.integer(forKey: "experience")
        
        if level == 0 { level = 1 }
    }
}

// MARK: - User Statistics

struct UserStatistics: Codable {
    var totalRegionsColored: Int = 0
    var totalStickersAdded: Int = 0
    var totalArtworksCreated: Int = 0
    var uniqueColorsUsed: Int = 0
    var customColorsCreated: Int = 0
    var templatesUsed: Set<String> = []
    var fastCompletions: Int = 0
    var perfectCompletions: Int = 0
    var consecutiveDays: Int = 0
    var lastActivityDate: Date = Date()
    var totalSessionTime: TimeInterval = 0
    var averageSessionTime: TimeInterval = 0
    var sessionsCount: Int = 0
    
    mutating func recordAction(_ action: UserAction) {
        switch action {
        case .colorRegion:
            totalRegionsColored += 1
        case .addSticker:
            totalStickersAdded += 1
        case .createArtwork:
            totalArtworksCreated += 1
        case .useColor(let color):
            // Track unique colors (simplified)
            uniqueColorsUsed = max(uniqueColorsUsed, uniqueColorsUsed + 1)
        case .createCustomColor:
            customColorsCreated += 1
        case .useTemplate(let template):
            templatesUsed.insert(template)
        case .fastCompletion:
            fastCompletions += 1
        case .perfectCompletion:
            perfectCompletions += 1
        case .sessionStart:
            sessionsCount += 1
        case .sessionEnd(let duration):
            totalSessionTime += duration
            averageSessionTime = totalSessionTime / Double(sessionsCount)
        }
        
        // Update consecutive days
        let today = Calendar.current.startOfDay(for: Date())
        let lastActivity = Calendar.current.startOfDay(for: lastActivityDate)
        
        if today == lastActivity {
            // Same day, no change
        } else if today == Calendar.current.date(byAdding: .day, value: 1, to: lastActivity) {
            // Next day, increment
            consecutiveDays += 1
        } else {
            // Gap in days, reset
            consecutiveDays = 1
        }
        
        lastActivityDate = Date()
    }
}

// MARK: - User Actions

enum UserAction {
    case colorRegion
    case addSticker
    case createArtwork
    case useColor(Color)
    case createCustomColor
    case useTemplate(String)
    case fastCompletion
    case perfectCompletion
    case sessionStart
    case sessionEnd(TimeInterval)
}

// MARK: - Achievements View

struct AchievementsView: View {
    @ObservedObject var progressTracker: ProgressTracker
    @State private var selectedRarity: AchievementRarity?
    
    var body: some View {
        VStack(spacing: 20) {
            // Header
            HStack {
                Text("🏆 Achievements")
                    .font(.title)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                Spacer()
                
                Text("Level \(progressTracker.level)")
                    .font(.headline)
                    .foregroundColor(.blue)
            }
            .padding(.horizontal)
            
            // Progress bar
            VStack {
                HStack {
                    Text("Experience: \(progressTracker.experience)")
                        .font(.subheadline)
                        .foregroundColor(.primary)
                    
                    Spacer()
                    
                    Text("Next Level: \(progressTracker.experience % 100)")
                        .font(.subheadline)
                        .foregroundColor(.secondary)
                }
                
                ProgressView(value: Double(progressTracker.experience % 100), total: 100)
                    .progressViewStyle(LinearProgressViewStyle(tint: .blue))
            }
            .padding(.horizontal)
            
            // Rarity filter
            ScrollView(.horizontal, showsIndicators: false) {
                HStack(spacing: 10) {
                    Button("All") {
                        selectedRarity = nil
                    }
                    .buttonStyle(FilterButtonStyle(isSelected: selectedRarity == nil))
                    
                    ForEach(AchievementRarity.allCases, id: \.self) { rarity in
                        Button(rarity.rawValue) {
                            selectedRarity = rarity
                        }
                        .buttonStyle(FilterButtonStyle(isSelected: selectedRarity == rarity))
                    }
                }
                .padding(.horizontal)
            }
            
            // Achievements list
            ScrollView {
                LazyVStack(spacing: 15) {
                    ForEach(filteredAchievements, id: \.type) { achievement in
                        AchievementRowView(achievement: achievement)
                    }
                }
                .padding(.horizontal)
            }
        }
        .padding(.vertical)
    }
    
    private var filteredAchievements: [AchievementType] {
        let allTypes = AchievementType.allCases
        if let rarity = selectedRarity {
            return allTypes.filter { $0.rarity == rarity }
        }
        return allTypes
    }
}

// MARK: - Achievement Row View

struct AchievementRowView: View {
    let achievement: AchievementType
    @ObservedObject var progressTracker: ProgressTracker
    
    private var userAchievement: Achievement? {
        progressTracker.achievements.first { $0.type == achievement }
    }
    
    private var isUnlocked: Bool {
        return userAchievement?.isCompleted ?? false
    }
    
    private var progress: Int {
        return userAchievement?.progress ?? progressTracker.getProgressForAchievement(achievement)
    }
    
    private var target: Int {
        return progressTracker.getTargetForAchievement(achievement)
    }
    
    var body: some View {
        HStack(spacing: 15) {
            // Icon
            ZStack {
                Circle()
                    .fill(isUnlocked ? achievement.rarity.color : Color.gray.opacity(0.3))
                    .frame(width: 50, height: 50)
                
                achievement.icon
                    .font(.system(size: 25))
                    .foregroundColor(isUnlocked ? .white : .gray)
            }
            
            // Content
            VStack(alignment: .leading, spacing: 5) {
                HStack {
                    Text(achievement.name)
                        .font(.headline)
                        .foregroundColor(.primary)
                    
                    Spacer()
                    
                    if isUnlocked {
                        Text("\(achievement.points) pts")
                            .font(.caption)
                            .foregroundColor(.secondary)
                    }
                }
                
                Text(achievement.description)
                    .font(.subheadline)
                    .foregroundColor(.secondary)
                
                // Progress bar
                if !isUnlocked {
                    VStack {
                        HStack {
                            Text("\(progress)/\(target)")
                                .font(.caption)
                                .foregroundColor(.secondary)
                            
                            Spacer()
                        }
                        
                        ProgressView(value: Double(progress), total: Double(target))
                            .progressViewStyle(LinearProgressViewStyle(tint: achievement.rarity.color))
                    }
                }
            }
            
            Spacer()
        }
        .padding()
        .background(
            RoundedRectangle(cornerRadius: 15)
                .fill(isUnlocked ? achievement.rarity.color.opacity(0.1) : Color.gray.opacity(0.05))
        )
        .overlay(
            RoundedRectangle(cornerRadius: 15)
                .stroke(isUnlocked ? achievement.rarity.color : Color.clear, lineWidth: 2)
        )
    }
}

// MARK: - Filter Button Style

struct FilterButtonStyle: ButtonStyle {
    let isSelected: Bool
    
    func makeBody(configuration: Configuration) -> some View {
        configuration.label
            .font(.subheadline)
            .foregroundColor(isSelected ? .white : .primary)
            .padding(.horizontal, 15)
            .padding(.vertical, 8)
            .background(
                RoundedRectangle(cornerRadius: 20)
                    .fill(isSelected ? Color.blue : Color.gray.opacity(0.2))
            )
            .scaleEffect(configuration.isPressed ? 0.95 : 1.0)
    }
}

// MARK: - Statistics View

struct StatisticsView: View {
    @ObservedObject var progressTracker: ProgressTracker
    
    var body: some View {
        VStack(spacing: 20) {
            Text("📊 Statistics")
                .font(.title)
                .fontWeight(.bold)
                .foregroundColor(.primary)
            
            LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 2), spacing: 15) {
                StatCardView(title: "Regions Colored", value: "\(progressTracker.statistics.totalRegionsColored)", icon: "paintbrush.fill", color: .blue)
                StatCardView(title: "Stickers Added", value: "\(progressTracker.statistics.totalStickersAdded)", icon: "sparkles", color: .purple)
                StatCardView(title: "Artworks Created", value: "\(progressTracker.statistics.totalArtworksCreated)", icon: "photo.fill", color: .green)
                StatCardView(title: "Templates Used", value: "\(progressTracker.statistics.templatesUsed.count)", icon: "square.grid.3x3.fill", color: .orange)
                StatCardView(title: "Custom Colors", value: "\(progressTracker.statistics.customColorsCreated)", icon: "drop.fill", color: .cyan)
                StatCardView(title: "Consecutive Days", value: "\(progressTracker.statistics.consecutiveDays)", icon: "calendar", color: .red)
            }
            .padding(.horizontal)
        }
        .padding(.vertical)
    }
}

// MARK: - Stat Card View

struct StatCardView: View {
    let title: String
    let value: String
    let icon: String
    let color: Color
    
    var body: some View {
        VStack(spacing: 10) {
            Image(systemName: icon)
                .font(.system(size: 30))
                .foregroundColor(color)
            
            Text(value)
                .font(.title)
                .fontWeight(.bold)
                .foregroundColor(.primary)
            
            Text(title)
                .font(.caption)
                .foregroundColor(.secondary)
                .multilineTextAlignment(.center)
        }
        .padding()
        .background(
            RoundedRectangle(cornerRadius: 15)
                .fill(color.opacity(0.1))
        )
        .overlay(
            RoundedRectangle(cornerRadius: 15)
                .stroke(color, lineWidth: 2)
        )
    }
}